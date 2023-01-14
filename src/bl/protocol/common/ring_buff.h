/*----------------------------------------------------------------------------*/
/**
* \file         ring_buff.h
* \brief        Cyclic buffer creation and manipulation functions for given type
*
* \date         20180509
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2018
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	ring_buffer
* \brief 	    Contains functions defined via macro to create and manipulate
*               cyclic buffers of given type, name and size.
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_RING_H_
#define SRC_RING_H_

// This header file should be included only in C files

#if defined(__cplusplus)
extern "C" {
#endif

// ===================================================================================================
// Define a ring (cyclic) buffer for a given type with supporting functions
// ===================================================================================================
typedef uint8_t span_t;

typedef struct
{
    uint8_t b_visited;
    uint8_t b_hidden;
} struct_RING_FEATURE;

typedef struct
{
    void * const ptr_buff;
    span_t head;
    span_t tail;
    span_t n;
    span_t const size;
    span_t const elem_size;
} struct_RING;

#define DEFINE_STATIC_RINGBUFFER(type, name, sz)    \
                                                    \
    typedef struct                                  \
    {                                               \
        type obj;                                   \
        struct_RING_FEATURE prop;                   \
    } type ## _ ## name ## _visitable;   \
                                                    \
    static type ## _ ## name ## _visitable mgl_ring_buff_ ## type ## _ ## name [sz]; \
    static struct_RING name = {                     \
        .ptr_buff = (void*)(& mgl_ring_buff_ ## type ## _ ## name [0]), \
        .head = 0u,                                 \
        .tail = 0u,                                 \
        .n = 0u,                                    \
        .size = sz,                                 \
        .elem_size = sizeof(type)                   \
    }


// ===================================================================================================
// Public function prototypes
// ===================================================================================================

/*----------------------------------------------------------------------------*/
/**
* \ingroup      ring_buffer
* \brief        Returns the number of available records
*
* \details
*
* \pre
*
* \param ptr_ring [in]      struct_RING*        pointer to the ring variable
* \return span_t                                Number of available records
*/
span_t    ringbuff_get_count        (struct_RING *ptr_ring) __attribute__((unused));

/*----------------------------------------------------------------------------*/
/**
* \ingroup      ring_buffer
* \brief        Resets the ring buffer to its initial state
*
* \details
*
* \pre
*
* \param ptr_ring [in]      struct_RING*        pointer to the ring variable
* \return void
*/
void      ringbuff_reset            (struct_RING *ptr_ring) __attribute__((unused));

/*----------------------------------------------------------------------------*/
/**
* \ingroup      ring_buffer
* \brief        Pushes a new record into the ring buffer
*
* \details
*
* \pre
*
* \param ptr_ring [in]      struct_RING*        pointer to the ring variable
* \param ptr_data [in]      void*               pointer to a data to be pushed into the ring varibale
* \param b_hidden [in]      uint8_t             marks the pushed data in a ring buffer as hidden (TRUE)
*                                               or not-hidden (FALSE). When hidden the pushed data remains
*                                               invisible until explicitly unhidden
* \return enum_CODE                             return code
*/
enum_CODE ringbuff_push             (struct_RING *ptr_ring, void *ptr_data, uint8_t b_hidden) __attribute__((unused));

/*----------------------------------------------------------------------------*/
/**
* \ingroup      ring_buffer
* \brief        Pops the oldest record out of the ring buffer and assigns it to provided data pointer
*
* \details
*
* \pre
*
* \param ptr_ring [in]      struct_RING*        pointer to the ring variable
* \param ptr_data [out]     void*               pointer to a data variable to write the popped record into
* \return enum_CODE                             return code
*/
enum_CODE ringbuff_pop              (struct_RING *ptr_ring, void *ptr_data) __attribute__((unused));

/*----------------------------------------------------------------------------*/
/**
* \ingroup      ring_buffer
* \brief        Read the oldest available record in the ring buffer without popping it (only marking it as "visited")
*
* \details
*
* \pre
*
* \param ptr_ring [in]      struct_RING*        pointer to the ring variable
* \param ptr_data [out]     void*               pointer to a data variable to write the popped record into
* \return enum_CODE                             return code
*/
enum_CODE ringbuff_read_shadow      (struct_RING *ptr_ring, void *ptr_data) __attribute__((unused));

/*----------------------------------------------------------------------------*/
/**
* \ingroup      ring_buffer
* \brief        Assigns the pointer to the oldest available record directly to an external pointer
*               Use this function with caution as unintented manipulation with the data record
*               may corrupt the ring buffer. This function is mostly provided as a more efficient
*               read-only variant of ringbuff_read_shadow function.
*
* \details
*
* \pre
*
* \param ptr_ring [in]      struct_RING*        pointer to the ring variable
* \param ptr_data [out]     void*               pointer to a data variable to write the popped record into
* \return enum_CODE                             return code
*/
enum_CODE ringbuff_read_shadow_ptr  (struct_RING *ptr_ring, void **ptr_ptr_data) __attribute__((unused));

/*----------------------------------------------------------------------------*/
/**
* \ingroup      ring_buffer
* \brief        Removes the oldest record if marked as "visited"
*
* \details
*
* \pre
*
* \param ptr_ring [in]      struct_RING*        pointer to the ring variable
* \return enum_CODE                             return code
*/
enum_CODE ringbuff_pop_if_visited   (struct_RING *ptr_ring) __attribute__((unused));

/*----------------------------------------------------------------------------*/
/**
* \ingroup      ring_buffer
* \brief        Checks if the oldest record is marked as "visited"
*
* \details
*
* \pre
*
* \param ptr_ring [in]      struct_RING*        pointer to the ring variable
* \return uint8_t                              TRUE - visited, FALSE - not visited
*/
uint8_t   ringbuff_is_node_visited  (struct_RING *ptr_ring) __attribute__((unused));

/*----------------------------------------------------------------------------*/
/**
* \ingroup      ring_buffer
* \brief        Unhides the oldest record
*
* \details
*
* \pre
*
* \param ptr_ring [in]      struct_RING*        pointer to the ring variable
* \return enum_CODE                             return code
*/
enum_CODE ringbuff_unhide_if_hidden (struct_RING *ptr_ring);


#ifdef __cplusplus
}
#endif

#endif /* SRC_RING_H_ */
