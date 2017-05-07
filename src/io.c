/*
 * io.c - I/O routines
 *
 * Specific to the TS-7200 ARM evaluation board
 *
 */

#include <ts7200.h>
#include <io.h>
#include <ringBuffer.h>
#include <trackControl.h>
#include <main.h>

/*
 * The UARTs are initialized by RedBoot to the following state
 *  115,200 bps
 *  8 bits
 *  no parity
 *  fifos enabled
 */
int* getUARTHighControl(int channel) {
    int *flags;
    switch( channel ) {
    case COM1:
        flags = (int *)( UART1_BASE + UART_LCRH_OFFSET );
        break;
    case COM2:
        flags = (int *)( UART2_BASE + UART_LCRH_OFFSET );
        break;
    default:
        return NULL;
        break;
    }
    return flags;
}

int setfifo( int channel, int state ) {
    int *line, buf;
    line = getUARTHighControl(channel);
    buf = *line;
    buf = state ? buf | FEN_MASK : buf & ~FEN_MASK;
    *line = buf;
    return 0;
}

int setSpeed( int channel, int speed ) {
    int *high, *low;
    switch( channel ) {
    case COM1:
        high = (int *)( UART1_BASE + UART_LCRM_OFFSET );
        low = (int *)( UART1_BASE + UART_LCRL_OFFSET );
            break;
    case COM2:
        high = (int *)( UART2_BASE + UART_LCRM_OFFSET );
        low = (int *)( UART2_BASE + UART_LCRL_OFFSET );
            break;
    default:
            return -1;
            break;
    }
    switch( speed ) {
    case 115200:
        *high = 0x0;
        *low = 0x3;  // 0000_0011
        return 0;
    case 2400:
        *high = 0x0;
        *low = 0xbf; // 1001_0000 = 144; 1010_1111 = 191.
        return 0;
    default:
        return -1;
    }
}

int* getUARTFlags(int channel) {
    int *flags;
    switch( channel ) {
    case COM1:
        flags = (int *)( UART1_BASE + UART_FLAG_OFFSET );
        break;
    case COM2:
        flags = (int *)( UART2_BASE + UART_FLAG_OFFSET );
        break;
    default:
        return NULL;
        break;
    }
    return flags;
}

int* getUARTData(int channel) {
    int *data;
    switch( channel ) {
    case COM1:
        data = (int *)( UART1_BASE + UART_DATA_OFFSET );
        break;
    case COM2:
        data = (int *)( UART2_BASE + UART_DATA_OFFSET );
        break;
    default:
        return NULL;
        break;
    }
    return data;
}

int putc( globalsStruct* globals, int channel, char c ) {
    ringBuffer_push(globals->outputBuffer, c);
    return 0;
}

// int putc( globalsStruct* globals, int channel, char c ) {
//     int *flags, *data;
//     flags = getUARTFlags(channel);
//     data = getUARTData(channel);
//     while( ( *flags & TXFF_MASK ) ) ;
//     *data = c;
//     return 0;
// }

void processOutputBuffer( globalsStruct* globals, int channel ) {
    int *flags, *data;
    flags = getUARTFlags(COM2);
    data = getUARTData(COM2);
    if (ringBuffer_hasElements(globals->outputBuffer) == 1 && (*flags & TXFE_MASK)) {
        *data = ringBuffer_pop(globals->outputBuffer);
    }
}

int processInputBuffer( globalsStruct* globals, int channel ) {
    int *flags, *data, flagsBuffer;
    unsigned char c;
    flags = getUARTFlags(COM2);
    data = getUARTData(COM2);

    if(*flags & RXFF_MASK) {
        c = *data;
        flagsBuffer = *flags;
        flagsBuffer = flagsBuffer & ~RXFF_MASK; // set 'Receive register full' to OFF
        flagsBuffer = flagsBuffer | RXFE_MASK;  // set 'Receive register empty' to ON
        *flags = flagsBuffer;

        if(c == 3) return -1; // Ctrl-C
        if(c != 13) { // Carriage return
            ringBuffer_push(globals->inputBuffer, c);
        } else {
            clearCommandPrompt(globals);
            printPreviousCommand(globals);
            globals->inputBuffer->readIndex = globals->inputBuffer->writeIndex;
            // runTrain(globals, 70, 12);
        }
    }
    return 0;
}

char c2x( char ch ) {
    if ( (ch <= 9) ) return '0' + ch;
    return 'a' + ch - 10;
}

int putx( globalsStruct* globals, int channel, char c ) {
    char chh, chl;

    chh = c2x( c / 16 );
    chl = c2x( c % 16 );
    putc( globals, channel, chh );
    return putc( globals, channel, chl );
}

int putr( globalsStruct* globals, int channel, unsigned int reg ) {
    int byte;
    char *ch = (char *) &reg;

    for( byte = 3; byte >= 0; byte-- ) putx( globals, channel, ch[byte] );
    return putc( globals, channel, ' ' );
}

int putstr( globalsStruct* globals, int channel, char *str ) {
    while( *str ) {
        if( putc( globals, channel, *str ) < 0 ) return -1;
        str++;
    }
    return 0;
}

void putw( globalsStruct* globals, int channel, int n, char fc, char *bf ) {
    char ch;
    char *p = bf;

    while( *p++ && n > 0 ) n--;
    while( n-- > 0 ) putc( globals, channel, fc );
    while( ( ch = *bf++ ) ) putc( globals, channel, ch );
}

int getc( globalsStruct* globals, int channel ) {
    int *flags, *data;
    unsigned char c;
    flags = getUARTFlags(channel);
    data = getUARTData(channel);
    // while ( !( *flags & RXFF_MASK ) ) ;
    // c = *data;
    return c;
}

int a2d( char ch ) {
    if( ch >= '0' && ch <= '9' ) return ch - '0';
    if( ch >= 'a' && ch <= 'f' ) return ch - 'a' + 10;
    if( ch >= 'A' && ch <= 'F' ) return ch - 'A' + 10;
    return -1;
}

char a2i( char ch, char **src, int base, int *nump ) {
    int num, digit;
    char *p;

    p = *src; num = 0;
    while( ( digit = a2d( ch ) ) >= 0 ) {
        if ( digit > base ) break;
        num = num*base + digit;
        ch = *p++;
    }
    *src = p; *nump = num;
    return ch;
}

void ui2a( unsigned int num, unsigned int base, char *bf ) {
    int n = 0;
    int dgt;
    unsigned int d = 1;
    
    while( (num / d) >= base ) d *= base;
    while( d != 0 ) {
        dgt = num / d;
        num %= d;
        d /= base;
        if( n || dgt > 0 || d == 0 ) {
            *bf++ = dgt + ( dgt < 10 ? '0' : 'a' - 10 );
            ++n;
        }
    }
    *bf = 0;
}

void i2a( int num, char *bf ) {
    if( num < 0 ) {
        num = -num;
        *bf++ = '-';
    }
    ui2a( num, 10, bf );
}

void format( globalsStruct* globals, int channel, char *fmt, va_list va ) {
    char bf[12];
    char ch, lz='0';
    int w;

    
    while ( ( ch = *(fmt++) ) ) {
        if ( ch != '%' )
            putc( globals, channel, ch );
        else {
            w = 1;
            ch = *(fmt++);
            switch ( ch ) {
            case '0':
                w++; ch = *(fmt++);
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                ch = a2i( ch, &fmt, 10, &w );
                break;
            }
            switch( ch ) {
            case 0: return;
            case 'c':
                putc( globals, channel, va_arg( va, char ) );
                break;
            case 's':
                putw( globals, channel, w, lz, va_arg( va, char* ) );
                break;
            case 'u':
                ui2a( va_arg( va, unsigned int ), 10, bf );
                putw( globals, channel, w, lz, bf );
                break;
            case 'd':
                i2a( va_arg( va, int ), bf );
                putw( globals, channel, w, lz, bf );
                break;
            case 'x':
                ui2a( va_arg( va, unsigned int ), 16, bf );
                putw( globals, channel, w, lz, bf );
                break;
            case '%':
                putc( globals, channel, ch );
                break;
            }
        }
    }
}

void bfprintf( globalsStruct* globals, int channel, char *fmt, ... ) {
        va_list va;

        va_start(va,fmt);
        format( globals, channel, fmt, va );
        va_end(va);
}

