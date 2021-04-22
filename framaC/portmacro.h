#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------*/

/* Type definitions. */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		int
#define portSHORT		short
#define portSTACK_TYPE  	unsigned long
#define portBASE_TYPE   	long

typedef portSTACK_TYPE   StackType_t;
typedef long             BaseType_t;
typedef unsigned long    UBaseType_t;

#if ( configUSE_16_BIT_TICKS == 1 )
typedef uint16_t     TickType_t;
# define portMAX_DELAY              ( TickType_t ) 0xffff
#else
typedef uint32_t     TickType_t;
# define portMAX_DELAY              ( TickType_t ) 0xffffffffUL

/* 32-bit tick type on a 32-bit architecture, so reads of the tick count do
* not need to be guarded with a critical section. */
# define portTICK_TYPE_IS_ATOMIC    1
#endif

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define portSTACK_GROWTH      ( -1 )
#if configTICK_RATE_HZ > 1000
# define portTICK_PERIOD_MS                    1 * ( ( TickType_t ) configTICK_RATE_HZ / 1000 )
#else
# define portTICK_PERIOD_MS                    ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#endif
#if CONFIG_TICK_RATE_HZ > 1000
# define portTICK_PERIOD_US                      1 + ((TickType_t) configTICK_RATE_HZ / 1000000)
#else
# define portTICK_PERIOD_US                     TICK_PERIOD_US_NOT_SUPPORED
#endif
#define portBYTE_ALIGNMENT    8
#define portDONT_DISCARD      __attribute__( ( used ) )

/*-----------------------------------------------------------*/

/* Scheduler utilities. */
extern void yield();
#define portYIELD() yield()

#define portEND_SWITCHING_ISR( xSwitchRequired )    if( xSwitchRequired != pdFALSE ) portYIELD()
#define portYIELD_FROM_ISR( x )                     portEND_SWITCHING_ISR( x )

/*-----------------------------------------------------------*/

/* Critical section management. */
extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );
#define portSET_INTERRUPT_MASK_FROM_ISR()         ulPortRaiseBASEPRI()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR( x )    vPortSetBASEPRI( x )
#define portDISABLE_INTERRUPTS()                  vPortRaiseBASEPRI()
#define portENABLE_INTERRUPTS()                   vPortSetBASEPRI( 0 )
#define portENTER_CRITICAL()                      vPortEnterCritical()
#define portEXIT_CRITICAL()                       vPortExitCritical()

/*-----------------------------------------------------------*/
/* Task function macros as described on the FreeRTOS.org WEB site.  These are
 * not necessary for to use this port.  They are defined so the common demo files
 * (which build with all the ports) will build. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters )    void vFunction( void * pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )          void vFunction( void * pvParameters )
/*-----------------------------------------------------------*/
/* Architecture specific optimisations. */
#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
# define configUSE_PORT_OPTIMISED_TASK_SELECTION    1
#endif
#if configUSE_PORT_OPTIMISED_TASK_SELECTION == 1
/* Generic helper function. */
uint8_t ucPortCountLeadingZeros( uint32_t ulBitmap );
/* Check the configuration. */
#if ( configMAX_PRIORITIES > 32 )
# error configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32.  It is very rare that a system requires more than 10 to 15 difference priorities as tasks that share a priority will time slice.
#endif

/* Store/clear the ready priorities in a bit map. */
#define portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities )    ( uxReadyPriorities ) |= ( 1UL << ( uxPriority ) )
#define portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities )     ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ) )

/*-----------------------------------------------------------*/

#define portGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities )    uxTopPriority = ( 31UL - ( uint32_t ) ucPortCountLeadingZeros( ( uxReadyPriorities ) ) )

#endif /* configUSE_PORT_OPTIMISED_TASK_SELECTION */

/*-----------------------------------------------------------*/
#ifdef configASSERT
void vPortValidateInterruptPriority( void );
# define portASSERT_IF_INTERRUPT_PRIORITY_INVALID()    vPortValidateInterruptPriority()
#endif

/* portNOP() is not required by this port. */
#define portNOP()

#define portINLINE              __inline

#ifndef portFORCE_INLINE
# define portFORCE_INLINE    inline __attribute__( ( always_inline ) )
#endif
BaseType_t xPortIsInsideInterrupt( void );

/*-----------------------------------------------------------*/

void vPortRaiseBASEPRI( void );
/*-----------------------------------------------------------*/
uint32_t ulPortRaiseBASEPRI( void );
/*-----------------------------------------------------------*/
void vPortSetBASEPRI( uint32_t ulNewMaskValue );
/*-----------------------------------------------------------*/
#define portMEMORY_BARRIER()    __asm volatile ( "" ::: "memory" )
/*-----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
