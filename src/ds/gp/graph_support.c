
#include "graph_support.h"
#include "graph_code.h"

#define MEMBITARRAY_VALUES  (MEMBITARRAY_INDICES)
#define MEM16ARRAY_VALUES   (MEMBITARRAY_VALUES+MEM16ARRAY_INDICES)
#define MEM32ARRAY_VALUES   (MEM16ARRAY_VALUES+MEM32ARRAY_INDICES)
#define CONST32ARRAY_VALUES (MEM32ARRAY_VALUES+CONST32ARRAY_INDICES)

index_type temp1,temp2;

extern uint8_t membitarray[MEMBITARRAY_SIZE];
extern uint16_t mem16array[MEM16ARRAY_SIZE];
extern uint32_t mem32array[MEM32ARRAY_SIZE];
extern const uint32_t const32array[CONST32ARRAY_SIZE];
const uint8_t bios_bitmask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

uint32_t get_max(index_type index)
  {
  if (index >= MEMBITARRAY_VALUES)
    if (index >= MEM16ARRAY_VALUES)
      if (index >= CONST32ARRAY_VALUES)
        return (uint32_t) 1;  
      else
#ifdef GRAPH_USE_DOUBLE
        return (uint32_t) 1e40; 
#else
        return (uint32_t) 0x7fffffff; 
#endif
    else
      return (uint32_t) 0x7fff; 
  else
    return 1;
  }

uint32_t get_min(index_type index)
  {
  if (index >= MEMBITARRAY_VALUES)
    if (index >= MEM16ARRAY_VALUES)
      if (index >= CONST32ARRAY_VALUES)
        return (uint32_t) 0;  
      else
#ifdef GRAPH_USE_DOUBLE
        return (uint32_t) -1e40; 
#else
        return (uint32_t) 0x80000000; 
#endif
    else
      return (uint32_t) 0x8000; 
  else
    return 1;
  }

uint32_t get(index_type index)
  {
  if (index >= MEMBITARRAY_VALUES)                        
    if (index >= MEM16ARRAY_VALUES)                       
      if (index >= MEM32ARRAY_VALUES)                     
        if (index >= CONST32ARRAY_VALUES)
          return (uint32_t) 0;  
        else
        {
          temp2 = index-MEM32ARRAY_VALUES;
          return (uint32_t) const32array[temp2]; 
        }          
      else
      {                
        temp1 = MEM16ARRAY_VALUES;
        temp2 = index-temp1; 
        return (uint32_t) mem32array[temp2]; 
      }      
    else
    {
      temp2 = index-MEMBITARRAY_VALUES;
      return (uint32_t) mem16array[temp2]; 
    }    
  else
    if(membitarray[index >> 3] & bios_bitmask[index & 7])
      return 1;
    else
      return 0;

  }



void set(index_type index, uint32_t value)
{
  index_type temp2;
  if (index >= MEMBITARRAY_VALUES)
  {
    if (index >= MEM16ARRAY_VALUES)
    {
      if (index >= MEM32ARRAY_VALUES)
        return; 
      else
      {
        temp2 = index-MEM16ARRAY_VALUES;
        mem32array[temp2] = value;  
        return;
      }
    }
    else
    { 
      temp2 = index-MEMBITARRAY_VALUES;  
      mem16array[temp2] = (uint16_t)(ROUND(value));  
      return;
    }
  }
  else
  {
    if((1 & ROUND(value)) > 0)
      membitarray[index >> 3] |= (bios_bitmask[index & 7]); 
    else
      membitarray[index >> 3] &= (~bios_bitmask[index & 7]); 
    return;
    }
  }




