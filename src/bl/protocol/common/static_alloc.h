/*----------------------------------------------------------------------------*/
/**
* \file         static_alloc.h
* \brief        Implementation of semi-dynamic allocation by partitioning static array.
*
* \date         20180509
* \author       DHakobyan
*
* \copyright    MRS Electronic GmbH & Co. KG 2018
*/
/*----------------------------------------------------------------------------*/
/**
* \defgroup   	static_alloc
* \brief 		Semi-dynamic (de-)allocation based on static array.
* \details
*/
/*----------------------------------------------------------------------------*/

#ifndef SRC_ROLE_PROTOCOL_ALLOC_H_
#define SRC_ROLE_PROTOCOL_ALLOC_H_

/*----------------------------------------------------------------------------*/
/**
* \ingroup		static_alloc
* \brief		Attemps to allocate the requested number of bytes as part of a
* 				pre-allocated static array.
* \details
*
* \pre
*
* \param size [in]		uint32_t		number of bytes to allocate
* \return void*							NULL on failure, otherwise the pointer to the allocated buffer
*/
void *static_alloc(uint32_t size);

/*----------------------------------------------------------------------------*/
/**
* \ingroup		static_alloc
* \brief		Frees the previously allocated buffer.
* \details
*
* \pre
*
* \param p [in]		void*		pointer to start of the previously allocated buffer
* \return void
*/
void static_free(void *p);

#endif /* SRC_ROLE_PROTOCOL_ALLOC_H_ */
