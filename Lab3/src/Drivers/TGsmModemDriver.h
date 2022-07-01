#ifndef T_GSM_MODEM_DRIVER_H_
#define T_GSM_MODEM_DRIVER_H_

#include <Core/Base/TGlobalPtrBase.h>
#include <HAL/TBufferedComInterfaceWriter.h>

#include <string.h>

/**
 * GSM modem driver that is capable of sending an SMS with AT commands.
 * @tparam T_COM_INTERFACE the communication interface class via which to communicate
 *   with the modem
 */
template <class T_COM_INTERFACE>
class TGsmModemDriver : public redBlocks::HAL::TBufferedComInterfaceWriter<T_COM_INTERFACE>
{
  public:

    typedef redBlocks::HAL::TBufferedComInterfaceWriter<T_COM_INTERFACE> BaseType;

    /**
     * Constructor
     */
    TGsmModemDriver() : BaseType( 220 )
    {
    }

    /**
     * Sends an SMS to the given number and with the given text
     * @param number the zero terminated phone number like this +499217877730
     * @param message the zero terminated message to be sent
     * @attention No length check is done within this method. The message must
     * not be larger than 160 characters, the number must not exceed 20 characters
     */
    static void sendSms( const char* number, const char* message )
    {
      const char data1[] = "AT+CMGF=1\n";
      const char data2[] = "AT+CMGS=\"";
      const char data3[] = "\"\n";
      const char data4 = 26;
      const char data5[] = "\n";
      BaseType::write( data1, strlen( data1 ) )&&
      BaseType::write( data2, strlen( data2 ) )&&
      BaseType::write( number, strlen( number ) )&&
      BaseType::write( data3, strlen( data3 ) )&&
      BaseType::write( message, strlen( message ) )&&
      BaseType::write( &data4, 1 )&&
      BaseType::write( data5, strlen( data5 ) );

    }
};

#endif // T_GSM_MODEM_DRIVER_H_
