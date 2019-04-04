// Terrortronics
// Bradley Elenbaas
// mr.elenbaas@gmail.com

#include "wiced.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define RX_WAIT_TIMEOUT        (1*SECONDS)
#define PORTNUM                (50007)           /* UDP port */
#define SEND_UDP_RESPONSE


/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/

static wiced_result_t process_received_udp_packet( );
static wiced_result_t send_udp_response(char* buffer, uint16_t buffer_length, wiced_ip_address_t ip_addr, uint32_t port);
void print_result(wiced_result_t someResult, char* someString);
void toggle_led();
wiced_result_t is_new_ip_address(wiced_ip_address_t);

/******************************************************
 *               Variable Definitions
 ******************************************************/

static const wiced_ip_setting_t device_init_ip_settings =
{
    INITIALISER_IPV4_ADDRESS( .ip_address, MAKE_IPV4_ADDRESS(192,168,  0,  1) ),
    INITIALISER_IPV4_ADDRESS( .netmask,    MAKE_IPV4_ADDRESS(255,255,255,  0) ),
    INITIALISER_IPV4_ADDRESS( .gateway,    MAKE_IPV4_ADDRESS(192,168,  0,  1) ),
};

static const wiced_ip_setting_t default_ip_settings =
{
    INITIALISER_IPV4_ADDRESS( .ip_address, MAKE_IPV4_ADDRESS(0, 0, 0, 0) ),
    INITIALISER_IPV4_ADDRESS( .netmask,    MAKE_IPV4_ADDRESS(0, 0, 0, 0) ),
    INITIALISER_IPV4_ADDRESS( .gateway,    MAKE_IPV4_ADDRESS(0, 0, 0, 0) ),
};

static wiced_timed_event_t process_udp_rx_event;
static wiced_udp_socket_t  udp_socket;

static wiced_ip_address_t ip_address_array_000;
static wiced_ip_address_t ip_address_array_001;
static wiced_ip_address_t ip_address_array[10];
static int counter = 0;
unsigned char c1;
unsigned char c2;


/******************************************************
 *               Function Definitions
 ******************************************************/

void application_start(void)
{
    // Declare...
    wiced_interface_t interface;
    wiced_result_t result;

    // Setup...
    wiced_init();                                                   // ... WICED library.

    result = wiced_network_up_default(                              // ... network.
            &interface,
            &device_init_ip_settings
    );
    print_result(result, "wiced_network_up_default");

    result = wiced_udp_create_socket(                               // ... UDP socket.
            &udp_socket,
            PORTNUM,
            interface
    );
    print_result(result, "wiced_udp_create_socket");

    wiced_rtos_register_timed_event(                                // ... thread.
            &process_udp_rx_event,
            WICED_NETWORKING_WORKER_THREAD,
            &process_received_udp_packet,
            1*SECONDS,
            0
    );
    print_result(result, "wiced_rtos_register_timed_event");

    WPRINT_APP_INFO(("success: application_start\n"));
}

void print_result(wiced_result_t someResult, char* someString)
{
    if (someResult == WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("success: "));
    }
    else
    {
        WPRINT_APP_INFO(("failure: "));
    }
    WPRINT_APP_INFO((someString));
    WPRINT_APP_INFO(("\n"));
}

void toggle_led()
{
    wiced_result_t result;

    result = wiced_gpio_input_get( WICED_LED1);

    if(result == WICED_TRUE)
    {
        wiced_gpio_output_low( WICED_LED1 );
    } else
    {
        wiced_gpio_output_high( WICED_LED1 );
    }
}

wiced_result_t process_received_udp_packet()
{
    wiced_packet_t*           packet;
    char*                     rx_data;
    static uint16_t           rx_data_length;
    uint16_t                  available_data_length;
    static wiced_ip_address_t udp_src_ip_addr;
    static uint16_t           udp_src_port;

    /* Wait for UDP packet */
    wiced_result_t result = wiced_udp_receive( &udp_socket, &packet, RX_WAIT_TIMEOUT );

    if ( ( result == WICED_ERROR ) || ( result == WICED_TIMEOUT ) )
    {
        return result;
    }

    /* Get info about the received UDP packet */
    wiced_udp_packet_get_info( packet, &udp_src_ip_addr, &udp_src_port );

    /* Extract the received data from the UDP packet */
    wiced_packet_get_data( packet, 0, (uint8_t**) &rx_data, &rx_data_length, &available_data_length );

    if (rx_data_length != available_data_length)
    {
        WPRINT_APP_INFO(("Fragmented packets not supproted\n"));
        return WICED_ERROR;
    }

    /* Null terminate the received data, just in case the sender didn't do this */
    rx_data[ rx_data_length ] = '\x0';

    /*
    WPRINT_APP_INFO ( ("UDP Rx: \"%s\" from IP %u.%u.%u.%u:%d\n", rx_data,
                                                                  (unsigned char) ( ( GET_IPV4_ADDRESS(udp_src_ip_addr) >> 24 ) & 0xff ),
                                                                  (unsigned char) ( ( GET_IPV4_ADDRESS(udp_src_ip_addr) >> 16 ) & 0xff ),
                                                                  (unsigned char) ( ( GET_IPV4_ADDRESS(udp_src_ip_addr) >>  8 ) & 0xff ),
                                                                  (unsigned char) ( ( GET_IPV4_ADDRESS(udp_src_ip_addr) >>  0 ) & 0xff ),
                                                                  udp_src_port ) );
                                                                  */
    WPRINT_APP_INFO ( ("IP %u.%u.%u.%u:%d\n",
                          (unsigned char) ( ( GET_IPV4_ADDRESS(udp_src_ip_addr) >> 24 ) & 0xff ),
                          (unsigned char) ( ( GET_IPV4_ADDRESS(udp_src_ip_addr) >> 16 ) & 0xff ),
                          (unsigned char) ( ( GET_IPV4_ADDRESS(udp_src_ip_addr) >>  8 ) & 0xff ),
                          (unsigned char) ( ( GET_IPV4_ADDRESS(udp_src_ip_addr) >>  0 ) & 0xff ),
                          udp_src_port ) );
    toggle_led();

    if(counter > 0)
    {
        result = is_new_ip_address(udp_src_ip_addr);
        if(result == WICED_SUCCESS)
        {
            WPRINT_APP_INFO((">>> IP ADDRESS NOT FOUND\n"));
            if(counter >= 9)
            {
                //break;
            }
            else
            {
                ip_address_array[counter] = udp_src_ip_addr;
            }
            ++counter;
        }
        else
        {
            WPRINT_APP_INFO((">>> IP ADDRESS FOUND\n"));
            /*
            if(counter >= 9)
            {
                //break;
            }
            else
            {
                ip_address_array[counter] = udp_src_ip_addr;
            }
            ++counter;
            */
        }
    }

    if (counter == 0)
    {
        //ip_address_array_000 = udp_src_ip_addr;
        ip_address_array[0] = udp_src_ip_addr;
        WPRINT_APP_INFO(("ADDING 0\n"));
        ++counter;
    }

    /*
    if (counter == 1)
    {
        c1 = (unsigned char) ( ( GET_IPV4_ADDRESS(udp_src_ip_addr) >>  0 ) & 0xff );
        c2 = (unsigned char) ( ( GET_IPV4_ADDRESS(ip_address_array_000) >>  0 ) & 0xff );
        if (c1 != c2)
        {
            ip_address_array_001 = udp_src_ip_addr;
            WPRINT_APP_INFO(("ADDING 1\n"));
            ++counter;
        }
    }
    */
    //++counter;

#ifdef SEND_UDP_RESPONSE
    /* Echo the received data to the sender */
    result = wiced_gpio_input_get( WICED_LED1);
    WPRINT_APP_INFO(("start\n"));
    for(int i = 0; i < 10; ++i)
    {
        /*
        WPRINT_APP_INFO ( ("--> %u.%u.%u.%u:%d\n",
                                                  (unsigned char) ( ( GET_IPV4_ADDRESS(ip_address_array[i]) >> 24 ) & 0xff ),
                                                  (unsigned char) ( ( GET_IPV4_ADDRESS(ip_address_array[i]) >> 16 ) & 0xff ),
                                                  (unsigned char) ( ( GET_IPV4_ADDRESS(ip_address_array[i]) >>  8 ) & 0xff ),
                                                  (unsigned char) ( ( GET_IPV4_ADDRESS(ip_address_array[i]) >>  0 ) & 0xff ),
                                                  udp_src_port ) );
        */
        //ip_address_array[i]
        if (result == WICED_SUCCESS)
        {
            send_udp_response( "on", 2, ip_address_array[i], PORTNUM );
        }
        else
        {
            send_udp_response( "off", 3, ip_address_array[i], PORTNUM );
        }
    }
    WPRINT_APP_INFO(("stop\n\n"));
    /*
    if (counter > 0)
    {
        if (wiced_gpio_input_get( WICED_LED1) == WICED_SUCCESS)
        {
            send_udp_response( "on", 2, ip_address_array_000, PORTNUM );
        }
        else
        {
            send_udp_response( "off", 3, ip_address_array_000, PORTNUM );
        }
    }
    if (counter > 1)
    {
        if (wiced_gpio_input_get( WICED_LED1) == WICED_SUCCESS)
        {
            send_udp_response( "on", 2, ip_address_array_001, PORTNUM );
        }
        else
        {
            send_udp_response( "off", 3, ip_address_array_001, PORTNUM );
        }
    }
    */
#endif

    /* Delete the received packet, it is no longer needed */
    wiced_packet_delete( packet );

    return WICED_SUCCESS;
}

wiced_result_t is_new_ip_address(wiced_ip_address_t some_ip_address)
{
    c1 = (unsigned char) ( ( GET_IPV4_ADDRESS(some_ip_address) >>  0 ) & 0xff );
    for (int i = 0; i < 10; ++i)
    {
        c2 = (unsigned char) ( ( GET_IPV4_ADDRESS(ip_address_array[i]) >>  0 ) & 0xff );

        WPRINT_APP_INFO( ("%x  >  %u ?= %u\n", i, c1, c2) );

        if (c1 == c2)
        {
            return WICED_ERROR;
        }
        else
        {
            //return WICED_SUCCESS;
        }
    }
    return WICED_SUCCESS;
}


static wiced_result_t send_udp_response (char* buffer, uint16_t buffer_length, wiced_ip_address_t ip_addr, uint32_t port)
{
    wiced_packet_t*          packet;
    char*                    tx_data;
    uint16_t                 available_data_length;
    const wiced_ip_address_t INITIALISER_IPV4_ADDRESS( target_ip_addr, GET_IPV4_ADDRESS(ip_addr) );

    /* Create the UDP packet. Memory for the TX data is automatically allocated */
    if ( wiced_packet_create_udp( &udp_socket, buffer_length, &packet, (uint8_t**) &tx_data, &available_data_length ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO( ("UDP tx packet creation failed\n") );
        return WICED_ERROR;
    }

    /* Copy buffer into tx_data which is located inside the UDP packet */
    memcpy( tx_data, buffer, buffer_length + 1 );

    /* Set the end of the data portion of the packet */
    wiced_packet_set_data_end( packet, (uint8_t*) tx_data + buffer_length );

    /* Send the UDP packet */
    if ( wiced_udp_send( &udp_socket, &target_ip_addr, port, packet ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO( ("UDP packet send failed\n") );
        /* Delete packet, since the send failed */
        wiced_packet_delete( packet );
    }
    else
    {
        //WPRINT_APP_INFO( ("UDP Tx: \"echo: %s\"\n", tx_data) ); // TODO: keep this
    }

    /*
     * NOTE : It is not necessary to delete the packet created above, the packet
     *        will be automatically deleted *AFTER* it has been successfully sent
     */

    return WICED_SUCCESS;
}
