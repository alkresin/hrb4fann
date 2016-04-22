
#include "hbapi.h"

#define HWG_USE_POINTER_ITEM

#ifdef HWG_USE_POINTER_ITEM
   #define HB_RETHANDLE( h )        hb_retptr( ( void * ) ( h ) )
   #define HB_PARHANDLE( n )        hb_parptr( n )
   #define HB_STOREHANDLE( h, n )   hb_storptr( ( void * ) ( h ), n )
   #define HB_PUTHANDLE( i, h )     hb_itemPutPtr( i, ( void * ) ( h ) )
   #define HB_GETHANDLE( i )        hb_itemGetPtr( i )
   #define HB_GETPTRHANDLE( i ,n )  hb_arrayGetPtr( i , n )
   #define HB_PUSHITEM( i )         hb_vmPushPointer( ( void * )i ) 
#else
   #define HB_RETHANDLE( h )        hb_retnl( ( HB_LONG ) ( h ) )
   #define HB_PARHANDLE( n )        ( ( HB_LONG ) hb_parnl( n ) )
   #define HB_STOREHANDLE( h, n )   hb_stornl( ( HB_LONG ) ( h ), n )
   #define HB_PUTHANDLE( i, h )     hb_itemPutNL( i, ( HB_LONG ) ( h ) )
   #define HB_GETHANDLE( i )        ( ( HB_LONG ) hb_itemGetNL( i ) )
   #define HB_GETPTRHANDLE( i ,n )  hb_arrayGetNL( i , n )
   #define HB_PUSHITEM( i )         hb_vmPushLong( ( HB_LONG )i ) 
#endif
