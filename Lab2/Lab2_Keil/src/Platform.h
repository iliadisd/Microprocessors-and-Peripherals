#ifndef PLATFORM_H_
#define PLATFORM_H_

void Platform_Led_setValue( unsigned char value );
void Platform_PushButton_setIsrCallback(void (*cbk)( void ));
unsigned char Platform_PushButton_getValue( void );
void Platform_PushButton_enableInterrupt( void );
void Platform_PushButton_disableInterrupt( void );
void Platform_registerSystickCallback(void (*cbk)( void ));


#endif /* PLATFORM_H_ */
