#ifndef HAL_CAN_H
#define HAL_CAN_H
/**
* \file         hal_can.h
* \brief        Definitions for a hardware independent CAN Interface
*/
/*----------------------------------------------------------------------------*/
/**
* \addtogroup   doc_hal_can HAL CAN
* @{
* \brief        This documentation part describes the interface function definitions for a hardware-independent and abstracted CAN bus interface.
* \details      Main goal is to create a hardware-platform independent layer, that can be accessed from different HW-platforms like
*               socketcan on LINUX, Renesas SSK Interface, Kinetis SDK.
*
*               Controller Area Network (CAN) is a multi master multi slave field bus with CSMA/CR on a 2-wire differential signal.
*
*               Short description for CAN implementations: \n
*               Depending on the specific implementation for can controllers (msCan, FlexCan, ...) there are different ways they work. Most common ways are:
*               - FIFO: Put all received can frames into a (hardware or software driven) FIFO buffer.
*               - Mailbox: Received CAN frames are filtered [eg. canID 0x12-0x25] and put into RX mailboxes; TX mailboxes send frames due to its priority setup.
*
*               To manage all different can controller settings, the individual characteristics need to be present in this definitions.
*               All implementation specific information can be found at its documentation.
*
*               For demo implementation see https://gitlab.mrs-electronic.de/HardwareAbstractionLayer/SW16029_HAL_DemoProject
*/
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// Inclusion of the following header is rather ugly
// But for the sake of keepnig the changes at minimal as comparared
// with the original code we leave it like this. The maintainer of the
// HAL should make the final decision on how to represent the
// low level types which are referenced in this file.

#include "hal_data_types.h"
#include "hal_version.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#define HAL_CAN_DLC_MAX         16u /* Maximum allowed data length code (DLC) of a CAN Message.
                                     * HAL is able to send CAN FD, so the max. value is elevated to this.
                                     * NOTE: DLC in CAN FD mode is not 1:1 bit encoded.
                                     * -> DLC 7 = 7 bytes; DLC 8 = 8 bytes; DLC 9 = 12 bytes etc. refer to CAN FD documentation.
                                    */

#define USE_PRECALCULTED_CAN_BAUDRATE_TABLES

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// Typedefs
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

/** This type lists all possible values for the Baudrate setup of the CAN interface. MRS default value is #HAL_CAN_BAUD_125 */
typedef enum
{
    HAL_CAN_BAUD_INVALID     = 0,       ///< Invalid Baudrate Selected. Do not use this value.
    HAL_CAN_BAUD_10             ,       ///< Selected Baudrate is 10 KBit/s
    HAL_CAN_BAUD_20             ,       ///< Selected Baudrate is 20 KBit/s
    HAL_CAN_BAUD_33             ,       ///< Selected Baudrate is 33 KBit/s
    HAL_CAN_BAUD_50             ,       ///< Selected Baudrate is 50 KBit/s
    HAL_CAN_BAUD_83             ,       ///< Selected Baudrate is 83 KBit/s
    HAL_CAN_BAUD_100            ,       ///< Selected Baudrate is 100 KBit/s
    HAL_CAN_BAUD_125            ,       ///< Selected Baudrate is 125 KBit/s
    HAL_CAN_BAUD_250            ,       ///< Selected Baudrate is 250 KBit/s
    HAL_CAN_BAUD_500            ,       ///< Selected Baudrate is 500 KBit/s
	HAL_CAN_BAUD_800            ,       ///< Selected Baudrate is 800 KBit/s
    HAL_CAN_BAUD_1000           ,       ///< Selected Baudrate is 1000 KBit/s
    HAL_CAN_BAUD_EXT            ,       ///< The baudrate for the application can be selected individually
    HAL_CAN_FD_BAUD_EXT         ,       ///< The baudrate is can-fd and can be selected individually
    HAL_CAN_BAUD_MAX                    ///< Maximum Baudrate Identifier. Do not use this value and leave at last position in enum.
}enum_HAL_CAN_BAUDRATE;

/** This type lists all possible error codes that can be returned when using the HAL-Can Functions. */
typedef enum
{
    HAL_CAN_OK                        = 0u,   ///< No Error. Command Succeeded.
    HAL_CAN_ERROR_GENERAL                 ,   ///< Not further described error code.
    HAL_CAN_ERROR_INIT_FAILED             ,   ///< Error opening a can communication channel of the given can interface.
    HAL_CAN_ERROR_NO_CHANNEL_LEFT         ,   ///< All channels of this interface are already in use (Resource busy).
    HAL_CAN_ERROR_NO_MESSAGE              ,   ///< No message available.
    HAL_CAN_ERROR_SET_FILTER              ,   ///< Error setting the can filter.
    HAL_CAN_ERROR_CHANNEL_INVALID         ,   ///< Given channel is not valid. This can happen when channel not opened yet.
    HAL_CAN_ERROR_WHILE_READING           ,   ///< Reading failed. Happens only when a message is available.
    HAL_CAN_ERROR_WHILE_WRITING           ,   ///< Writing failed.
    HAL_CAN_ERROR_BUSY                    ,   ///< Given channel is actually busy.
    HAL_CAN_ERROR_READ_EMPTY              ,   ///< The given receive channel does not contain data (channel is empty).
    HAL_CAN_ERROR_READ_RUN                ,   ///< Receive channel was overwritten (Overflow).
    HAL_CAN_ERROR_SET_CALLBACK            ,   ///< Error at setting the callback.
    HAL_CAN_ERROR_BAUDRATE                ,   ///< Error at setting the baudrate.
    HAL_CAN_ERROR_NOT_IMPLEMENTED         ,   ///< Function not implemented.
    HAL_CAN_ERROR_DLC_INVALID                 ///< Invalid DLC.

} enum_HAL_CAN_RETURN_VALUE;

/** This type lists all possible values that can be used for a can message frame direction. */
typedef enum
{
    HAL_CAN_RX                        = 0u,   ///< Receive direction (RX).
    HAL_CAN_TX                                ///< Transmit direction (TX).
} enum_HAL_CAN_DIRECTION;


/** The 32 Bit Can identifier has a special format:  \n
* \image html can_id.png
*
* | Bit(s)  | Description                                                   |
* |---------|---------------------------------------------------------------|
* |bit 31   | frame format flag (0 = standard 11 bit, 1 = extended 29 bit)  |
* |bit 30   | remote transmission request flag (1 = rtr frame)              |
* |bit 29   | error message frame flag (0 = data frame, 1 = error message)  |
* |bit 28-0 | CAN identifier (11/29 bit), start from bit 0                  |
*/
typedef uint32_t hal_canid_t;

/** This struct contains all information for a complete can frame.*/
typedef struct
{
    hal_canid_t  can_id;            ///< CAN Address and Flags, bit 31 is extended id flag. see \ref hal_canid_t. Normally just use this for CAN Address.
    uint8_t      can_dlc;           ///< DataLengthCode: Number of bytes in can frame. Maximum value for normal can is 8 byte per frame.
    uint8_t      can_fd;            ///< message is can-fd message
    uint8_t      can_fd_brs;        ///< message is can-fd message with bit rate switching
    uint8_t      can_fd_esi;        ///< message is can-fd message with error active
    uint8_t*     ptr_data;          ///< data content of the can frame, needs to point to a data array with appropriate payload size.
}struct_hal_can_frame;

typedef struct {
    uint32_t prop_seg;         /*!< Propagation segment*/
    uint32_t phase_seg1;       /*!< Phase segment 1*/
    uint32_t phase_seg2;       /*!< Phase segment 2*/
    uint32_t pre_divider;      /*!< Clock prescaler division factor*/
    uint32_t jumpwidth;        /*!< Resync jump width*/
} hal_can_time_segment;


/** \brief Function pointer type declaration for the function that is called, when a new can frame was received.
*   \details You have to implement a function in your application to get notified when a can frame received:
*   At this point it is not defined when exactly this function is called (cyclic call or directly in irq)- this is implementation specific.
*   Here just the function pointers format is declared.
*   example \n
*   \code
    // This function is called when a new can frame was received.
    enum_HAL_CAN_RETURN_VALUE my_can_receive_function(struct_hal_can_frame* ptr_can_msg)
    {
        // handle received can message
        if(ptr_can_msg->can_id == 0x123)
        {
           // do something
        }
    }
    \endcode
*   \param ptr_can_msg    Pointer to the message that was actually received.
*   \return               enum_HAL_CAN_RETURN_VALUE. Returncode for low level drivers. Unused at this time. Use always 0.
*/
typedef void (*hal_can_rx_callback_function_type)(struct_hal_can_frame* ptr_can_msg);

/// Typ für eine Callbackfunction zur Benachrichtigung bei CAN Bus Fehler
typedef void (*hal_can_err_callback_function_type)(uint16_t can_error);


/** Handler structure for a single can interface.
*   This is for internal usage only.
*   Each can interface must have its own handler structure.
*   An instance of this structure is passed at the initialization function (see #hal_can_init) and automatically filled with the needed information.
*   Then you can use this handler to identify the module where a can action has to be performed (eg. #hal_can_send).
*   \internal
*   Valid is either handle pointer or handle number.
*   \endinternal
*/
typedef struct
{
    void*   can_handle_ptr;                             ///< Structure for can channel identification. A pointer on handle itself.
    int32_t can_handle_number;                          ///< CAN channel identification number. This is equal to can bus number.
    int32_t can_handle_mb_idx;                          ///< Number to identify a dedicated mailbox / message box. Can be used by low level driver but may not.
    enum_HAL_CAN_BAUDRATE can_baudrate;                 ///< Transmission ratio for can transfers. Use an enum from #enum_HAL_CAN_BAUDRATE.
    enum_HAL_CAN_BAUDRATE can_fd_data_baudrate;         ///< Baud rate for CAN FD data phase.
    enum_HAL_CAN_DIRECTION can_handle_direction;        ///< Receive or transmit direction of channel or given mailbox / message box. Use an enum from #enum_HAL_CAN_DIRECTION.
    hal_can_rx_callback_function_type rx_cb_function;   ///< function pointer for receive callback
    hal_can_err_callback_function_type err_cb_function; ///< function pointer for err callback
    hal_can_time_segment can_ext_baudrate;              ///< can hs or can-fd arbitration phase flexcan_time_segment_t
    hal_can_time_segment can_fd_ext_baudrate;           ///< can-fd data phase flexcan_time_segment_t
    uint8_t is_can_fd;                                  ///< defines if can gets initialized as can-fd or can-hs mode
} struct_hal_can_handle;

/** Struct for can filter setup configuration. */
typedef struct
{
    hal_canid_t  can_id;                                ///< ID on which to filter
    hal_canid_t  can_mask;                              ///< Mask for can filter: Just the used Bits are really filtered.
    uint8_t id_type;					                ///< "0" Standard or "1" extended id type -> flexcan_msgbuff_id_type_t
} struct_hal_can_filter;


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// Prototypes
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/**
*   \brief Initialize a can interface (channel).
*   \details    Initializes the can interface channel that is mapped to the given bus id. Then the handle is returned to the can_handle pointer.
*   It is possible to call this function multiple times on same interface to allow different filter setups.
*   Depending on the implementation of this function, a fifo or message box system is used.\n
*   Example:  Pass a pointer to the (uninitialized) can handle and device HW is initialized.  The output variable is now filled with a handle.
*   \code
    #define  CAN_0  0u

    struct_hal_can_handle can_handle;
    enum_HAL_CAN_RETURN_VALUE ret;

    ret = hal_can_init(&can_handle, CAN_0, FALSE);
    \endcode
*   \param ptr_can_handle [inout] #struct_hal_can_handle* A pointer to an uninitialized (new) can handle that will be filled with the handler data.
*   \param bus_id         [in] uint8_t Numerical Identifier (can bus identifier) of the can interface that will be initialized.
*   \param block_mode     [in] _Bool Receive data blocking mode selection: \n
*                               true  = Read calls are blocked until a can frame is received.\n
*                               false = Read calls are not blocked, the returned result indicates if a can frame was received.
*
*   \return               Result of the function: Either be zero for success or a positive number from #enum_HAL_CAN_RETURN_VALUE as error code.
*
*
*/
enum_HAL_CAN_RETURN_VALUE hal_can_init(struct_hal_can_handle* ptr_can_handle, uint8_t bus_id, _Bool block_mode);



/*----------------------------------------------------------------------------*/
/**
*   \brief De-initializing of a given can interface (or channel).
*   \details The interface or channel for the given handler will be closed.
*   Because each can interface can be connected to multiple channels, the interface should only be closed when no other handler accesses the interface.
*
*   \param ptr_can_handle [in] #struct_hal_can_handle* A Pointer to the handler of the channel that will be closed.
*
*   \return               Result of the function: Either be zero for success or a positive number from #enum_HAL_CAN_RETURN_VALUE as error code.
*/
enum_HAL_CAN_RETURN_VALUE hal_can_deinit(struct_hal_can_handle* ptr_can_handle);


/*----------------------------------------------------------------------------*/
/**
*   \brief Write a can frame.
*   \details A can frame is passed to the driver which will send it on the given can interface/channel.\n
*   Example: Send data "00 11 22 33 44 55 66 77" on Can ID 0x100 for channel can0. \n
*   \code
    #define  CAN_0  0u

    struct_hal_can_handle can_handle;
    enum_HAL_CAN_RETURN_VALUE ret;
    struct_hal_can_frame frame_to_send;

    frame_to_send.can_id = 0x100;
    frame_to_send.can_dlc = 8u;
    frame_to_send.data[0] = 0x00;
    frame_to_send.data[1] = 0x11;
    frame_to_send.data[2] = 0x22;
    frame_to_send.data[3] = 0x33;
    frame_to_send.data[4] = 0x44;
    frame_to_send.data[5] = 0x55;
    frame_to_send.data[6] = 0x66;
    frame_to_send.data[7] = 0x77;

    ret = hal_can_init(&can_handle, CAN_0, FALSE);
    if( ret == 0 )
    {
       ret = hal_can_send(&can_handle,
    }
    \endcode
*
*   \pre    The function #hal_can_init must be called once before you can use this.
*
*   \param ptr_can_handle [in] #struct_hal_can_handle* Pointer to the can handle that maps the interface channel on which to send the frame to.
*   \param ptr_can_msg    [in] #struct_hal_can_frame* A Pointer to the can frame that should be transmitted.
*
*   \return               Result of the function: Either be zero for success or a positive number from #enum_HAL_CAN_RETURN_VALUE as error code.

*/
enum_HAL_CAN_RETURN_VALUE hal_can_send(const struct_hal_can_handle* ptr_can_handle, const struct_hal_can_frame* ptr_can_msg);


/*----------------------------------------------------------------------------*/
/**
*   \brief Read a Can frame.
*   \details Depending on the selection you have done at the #hal_can_init with parameter blocking_mode, you have either a blocking read until arrived or
*   a non blocking function that returns #HAL_CAN_ERROR_NO_MESSAGE when nothing is available.
*
*   \pre    The function #hal_can_init must be called once before you can use this.
*
*   \param ptr_can_handle      [in] #struct_hal_can_handle* Pointer to the handler from which channel the data have to be read.
*   \param ptr_can_msg         [out] #struct_hal_can_frame* A Pointer to a message frame structure where the received frame will be stored.
*
* \return                    Result of the function: Either be zero for success or a positive number from #enum_HAL_CAN_RETURN_VALUE as error code.
*/
enum_HAL_CAN_RETURN_VALUE hal_can_receive(const struct_hal_can_handle* ptr_can_handle, struct_hal_can_frame* ptr_can_msg);

/*----------------------------------------------------------------------------*/
/**
*   \brief Set an ID-Filter for a given can interface channel.
*   \details Most can interfaces have a can id filtering available, that let only pass can frames which are in the filters range.
*   Please perform this action after the init function or data will be lost.
*
*   \pre    The function #hal_can_init must be called once before you can use this.
*
*   \param ptr_can_handle  [in] #struct_hal_can_handle* Pointer to the handler of which channel the filter have to be set.
*   \param ptr_can_filter  [in] #struct_hal_can_filter* Pointer to the structure that contains the filter setup.
*
*   \return                Result of the function: Either be zero for success or a positive number from #enum_HAL_CAN_RETURN_VALUE as error code.
*/
enum_HAL_CAN_RETURN_VALUE hal_can_set_filter(const struct_hal_can_handle* ptr_can_handle, const struct_hal_can_filter* ptr_can_filter);

/*----------------------------------------------------------------------------*/
/**
*   \brief Obtains configured RX mailbox id type by analyzing the respective mask register.
*   \details Returns 0 if the RX mailbox is configured as standard frame filter or 1 if as extended filter
*   Please perform this action after the init and set_filter functions or it will result in undefined behavior.
*
*   \pre    The functions #hal_can_init and #hal_can_set_filter must be called once before you can use this.
*
*   \param ptr_can_handle  [in] #struct_hal_can_handle* Pointer to the handler of which channel the filter have to be set.
*   \param mb_idx          [in] #uint32_t Unsigned integer of the mailbox index which to be checked
*
*   \return                Result of the function: false for standard frame and true for extended frame.
*/
bool hal_can_is_mb_filter_extended(const struct_hal_can_handle* ptr_can_handle, uint32_t mb_idx);

/*----------------------------------------------------------------------------*/
/**
*   \brief Enables or modifies the callback function that is called whenever a new can frame was received.
*   \details Each channel can be mapped to another callback function. The receive of a can message for selected handler leads to a call of the given function. \n
*   Example: Setup callback \n
*   \code
    enum_HAL_CAN_RETURN_VALUE my_can_receive_function(struct_hal_can_frame* ptr_can_msg)
    {
        // do something
    }

    ...

    ret = hal_can_set_callback(&can_handle, my_can_receive_function );
    \endcode
*
*   \pre    The function #hal_can_init must be called once before you can use this.
*
*   \param ptr_can_handle   [in] #struct_hal_can_handle*             Pointer to the selected can interface channel handler.
*   \param ptr_cb_function  [in] #hal_can_callback_function_type    Pointer to the callback function
*
*   \return                  Result of the function: Either be zero for success or a positive number from #enum_HAL_CAN_RETURN_VALUE as error code.
*/
enum_HAL_CAN_RETURN_VALUE hal_can_set_receive_callback(struct_hal_can_handle* ptr_can_handle, hal_can_rx_callback_function_type ptr_cb_function);

/// \brief Setzen einer Callbackroutine für die Erkennung von CAN Bus Fehler auf einen CAN Kanal
///
///  Ein CAN Bus Fehler auf dem CAN Kanal führt zum Aufruf dieser Callbackfunktion.
///
/// \param ptr_can_handle    Zeiger auf den Handle des anzusprechenden CAN Kanals
/// \param callback_function Zeiger auf die Callbackfunktion
///
/// \return                  Ergebnis des Aufrufs als Erfolgs- oder Fehlercode
///
enum_HAL_CAN_RETURN_VALUE hal_can_set_error_callback(struct_hal_can_handle* ptr_can_handle, hal_can_err_callback_function_type ptr_cb_function);


/*----------------------------------------------------------------------------*/
/**
*   \brief Internal debug function
*   \details Print out a can message depending on platform. Mostly printf.
*
*   \param ptr_can_msg    [in] #struct_hal_can_frame* A Pointer to the can frame.
*
*   \return                Result of the function: Either be zero for success or a positive number from #enum_HAL_CAN_RETURN_VALUE as error code.
*/
enum_HAL_CAN_RETURN_VALUE hal_can_dump(const struct_hal_can_frame* ptr_can_msg);


/*----------------------------------------------------------------------------*/
/**
*   \brief Set the baud rate of the given channel.
*   \details Only values from #enum_HAL_CAN_BAUDRATE ( 1000kBaud, 500kBaud, 250kBaud, 125kBaud, 100kBaud, 83,3kBaud, 50kBaud, 33,3kBaud ) are possible.
*
*   \pre    The function #hal_can_init must be called once before you can use this.
*
*   \param ptr_can_handle  [in] #struct_hal_can_handle* Pointer to the handler that contains the can interface of which the baud rate is to change.
*   \param baudrate        [in] #enum_HAL_CAN_BAUDRATE New baud rate.
*
*   \return                Result of the function: Either be zero for success or a positive number from #enum_HAL_CAN_RETURN_VALUE as error code.
*/
enum_HAL_CAN_RETURN_VALUE hal_can_set_baudrate(struct_hal_can_handle* ptr_can_handle, enum_HAL_CAN_BAUDRATE baudrate);

/*----------------------------------------------------------------------------*/
/**
*   \brief Set the baud rate of the given channel for the CAN FD data phase.
*   \details Only values from #enum_HAL_CAN_BAUDRATE ( 5000kBaud, 4000kBaud, 2000kBaud, 1000kBaud, 500kBaud, 250kBaud, 125kBaud, 100kBaud, 83,3kBaud, 50kBaud, 33,3kBaud ) are possible.
*
*   \pre    The function #hal_can_init must be called once before you can use this.
*
*   \param ptr_can_handle  [in] #struct_hal_can_handle* Pointer to the handler that contains the can interface of which the baud rate is to change.
*   \param baudrate        [in] #enum_HAL_CAN_BAUDRATE New baud rate.
*
*   \return                Result of the function: Either be zero for success or a positive number from #enum_HAL_CAN_RETURN_VALUE as error code.
*/
enum_HAL_CAN_RETURN_VALUE hal_can_set_fd_data_baudrate(struct_hal_can_handle* ptr_can_handle, enum_HAL_CAN_BAUDRATE baudrate);

/*----------------------------------------------------------------------------*/
/**
*   \brief Configures internal TX structure parameters.
*   \details This function should be used together with CAN transmit function.
*
*   \param ptr_tx_info  [in]	#flexcan_data_info_t* Pointer to the structure that contains the can TX parameters.
*
*   \return                		void.
*/
enum_HAL_CAN_RETURN_VALUE hal_can_set_tx_mode(struct_hal_can_handle* ptr_can_handle, bool enable_fd, uint8_t fd_padding, bool enable_brs);

/*----------------------------------------------------------------------------*/
/**
*   \brief Read the actual baud rate of the given channel.
*   \details Only values from #enum_HAL_CAN_BAUDRATE ( 1000kBaud, 500kBaud, 250kBaud, 125kBaud, 100kBaud, 83,3kBaud, 50kBaud, 33,3kBaud ) are readable.
*
*   \param ptr_can_handle  Zeiger auf die CAN Nachricht
*   \param baudrate*       [in] #struct_hal_can_handle* Pointer to the handler that contains the can interface of which the baud rate is to read.
*
*   \return                Result of the function: Either be zero for success or a positive number from #enum_HAL_CAN_RETURN_VALUE as error code.
*/
enum_HAL_CAN_RETURN_VALUE hal_can_get_baudrate(struct_hal_can_handle* ptr_can_handle, enum_HAL_CAN_BAUDRATE* baudrate);

/// \brief re-aktiviert den CAN bus nach einem Fehler Event (z.B. buss off)
///
/// \param ptr_can_handle  Zeiger auf die CAN Nachricht
///
/// \return                Ergebnis des Aufrufs als Erfolgs- oder Fehlercode
///
enum_HAL_CAN_RETURN_VALUE hal_can_recover_from_bus_error(struct_hal_can_handle* ptr_can_handle);

/*----------------------------------------------------------------------------*/
/**
*   \brief Returns next message box index for transmission
*   \details
*
*   \pre
*
*   \param can_handle_number  [in] #uint32_t Bus index
*
*   \return                Returns the message box index
*/
uint8_t hal_can_get_msg_box_idx(uint32_t can_handle_number);

/*----------------------------------------------------------------------------*/
/**
*   \brief Configures a message box for transmission
*   \details
*
*   \pre
*
*   \param can_handle  [in] #struct_hal_can_handle* Pointer to the handler that contains the can interface of which the baud rate is to change.
*   \param can_id_ext  [in] #uint8_t Type of frame (i.e. externded or standard)
*   \param msg_len_dlc  [in] #uint8_t Frame length (i.e. number of data bytes per frame)
*   \param can_id  [in] #uint32_t ID of the frame
*
*   \return                Returns the message box index
*/
//void hal_can_set_tx_message_box(struct_hal_can_handle* ptr_can_handle, uint8_t can_id_ext, uint8_t msg_len_dlc, uint32_t can_id);


/*----------------------------------------------------------------------------*/
/**
*   \brief  Helper function to convert payload length to DLC
*   \details
*
*   \pre
*
*   \param len     [in]     uint8_t the number of bytes in the payload
*
*   \return                 the value converted to dlc
*/
uint8_t hal_can_len_to_dlc( uint8_t len );


/*----------------------------------------------------------------------------*/
/**
*   \brief  Helper function to convert DLC to length of the payload
*   \details
*
*   \pre
*
*   \param dlc   [in]       uint8_t Value found in the can frame
*
*   \return                 the value converted to length
*/
uint8_t hal_can_dlc_to_len( uint8_t dlc );

/** @}*/
#endif // _HAL_CAN_H_

