#ifndef TFC_UART_H_
#define TFC_UART_H_

void TFC_InitUARTs();

void TFC_UART_Process();
void uart_printint();
void uart_putchar (char ch);
void uart_printint(int num);

extern ByteQueue SDA_SERIAL_OUTGOING_QUEUE;
extern ByteQueue SDA_SERIAL_INCOMING_QUEUE;


#endif /* TFC_UART_H_ */
