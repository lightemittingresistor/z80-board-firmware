/******************************************************************************
 * z80-board-firmware
 * hex-receiver.c - code to receive Intel Hex files
 *
 *****************************************************************************/

#include "hex-receiver.h"
#include "stringtools.h"

void hex_init(hex_parser_context* ctx)
{
    ctx->state = HEX_PARSER_STATE_INIT;
    ctx->section_digit = 0;

    ctx->byte_count = 0;
    ctx->address = 0;
    ctx->record_type = 0;
    ctx->rxcount = 0;
    ctx->checksum = 0;
    ctx->calculated_checksum = 0;
    ctx->checksum_digit = 0;
    ctx->checksum_scratch = 0;
}

void hex_parse_char(hex_parser_context* ctx, char c)
{
    if(ctx->state != HEX_PARSER_STATE_CHECKSUM &&
        ctx->state != HEX_PARSER_STATE_INIT)
    {
        if(ctx->checksum_digit++ == 0)
        {
            ctx->checksum_scratch = hexdigittoint(c);
        }
        else
        {
            ctx->calculated_checksum += (ctx->checksum_scratch << 4) + hexdigittoint(c);
            ctx->checksum_digit = 0;
            ctx->checksum_scratch = 0;
        }
    }

    switch(ctx->state)
    {
        case HEX_PARSER_STATE_DONE:
        case HEX_PARSER_STATE_GOT_DATA:
        {
            hex_init(ctx);
        } // fall through to INIT here
        case HEX_PARSER_STATE_INIT:
        {
            if(c == ':')
            {
                ctx->state = HEX_PARSER_STATE_WAIT_HEADER;
                ctx->section_digit = 0;
            }
        } break;
        case HEX_PARSER_STATE_WAIT_HEADER:
        {
            switch(ctx->section_digit)
            {
                // byte count
                case 0:
                case 1:
                {
                    ctx->byte_count = (ctx->byte_count << 4) + hexdigittoint(c);
                } break;
                //address
                case 2:
                case 3:
                case 4:
                case 5:
                {
                    ctx->address = (ctx->address << 4) + hexdigittoint(c);
                } break;
                // record type
                case 6:
                case 7:
                {
                    ctx->record_type = (ctx->record_type << 4) + hexdigittoint(c);
                } break;
            };
            
            if(++ctx->section_digit == 8)
            {
                if(ctx->byte_count > 0)
                {
                    ctx->state = HEX_PARSER_STATE_WAIT_DATA;
                }
                else
                {
                    ctx->state = HEX_PARSER_STATE_CHECKSUM;
                }
                ctx->section_digit = 0;
            }
        } break;
        case HEX_PARSER_STATE_WAIT_DATA:
        {
            if(ctx->section_digit == 0)
            {
                ctx->data[ctx->rxcount] = hexdigittoint(c);
                ++ctx->section_digit;
            }
            else
            {
                ctx->data[ctx->rxcount] = (ctx->data[ctx->rxcount] << 4) + hexdigittoint(c);
                ctx->section_digit = 0;
                ++ctx->rxcount;
                if(ctx->rxcount == ctx->byte_count)
                {
                    ctx->state = HEX_PARSER_STATE_CHECKSUM;
                }
            }
        } break;
        case HEX_PARSER_STATE_CHECKSUM:
        {
            ctx->checksum  = (ctx->checksum << 4) + (unsigned char)hexdigittoint(c);
            if(ctx->section_digit == 0)
            {
                ctx->section_digit = 1;
            }
            else
            {
                ctx->calculated_checksum = ~ctx->calculated_checksum;
                ctx->calculated_checksum += 1;

                ctx->state = HEX_PARSER_STATE_GOT_DATA;
                if(ctx->record_type == HEX_RECORD_EOF)
                {
                    ctx->state = HEX_PARSER_STATE_DONE;
                }
            }
        } break;
    }
}

bool hex_parse_check_checksum(hex_parser_context* ctx)
{
    return ctx->calculated_checksum == ctx->checksum;
}