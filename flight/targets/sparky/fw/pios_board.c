/**
 ******************************************************************************
 * @addtogroup TauLabsTargets Tau Labs Targets
 * @{
 * @addtogroup Sparky Tau Labs Sparky support files
 * @{
 *
 * @file       pios_board.c
 * @author     Tau Labs, http://taulabs.org, Copyright (C) 2012-2014
 * @brief      The board specific initialization routines
 * @see        The GNU Public License (GPL) Version 3
 * 
 *****************************************************************************/
/* 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 3 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 * for more details.
 * 
 * You should have received a copy of the GNU General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* Pull in the board-specific static HW definitions.
 * Including .c files is a bit ugly but this allows all of
 * the HW definitions to be const and static to limit their
 * scope.  
 *
 * NOTE: THIS IS THE ONLY PLACE THAT SHOULD EVER INCLUDE THIS FILE
 */

#include "board_hw_defs.c"

#include <pios.h>
#include <openpilot.h>
#include <uavobjectsinit.h>
#include "hwsparky.h"
#include "manualcontrolsettings.h"
#include "modulesettings.h"

/**
 * Configuration for the MS5611 chip
 */
#if defined(PIOS_INCLUDE_MS5611)
#include "pios_ms5611_priv.h"
static const struct pios_ms5611_cfg pios_ms5611_cfg = {
	.oversampling = MS5611_OSR_1024,
	.temperature_interleaving = 1,
};
#endif /* PIOS_INCLUDE_MS5611 */

/**
 * Configuration for the MPU6050 chip
 */
#if defined(PIOS_INCLUDE_MPU6050)
#include "pios_mpu6050.h"
static const struct pios_exti_cfg pios_exti_mpu6050_cfg __exti_config = {
	.vector = PIOS_MPU6050_IRQHandler,
	.line = EXTI_Line15,
	.pin = {
		.gpio = GPIOA,
		.init = {
			.GPIO_Pin = GPIO_Pin_15,
			.GPIO_Speed = GPIO_Speed_50MHz,
			.GPIO_Mode = GPIO_Mode_IN,
			.GPIO_OType = GPIO_OType_OD,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		},
	},
	.irq = {
		.init = {
			.NVIC_IRQChannel = EXTI15_10_IRQn,
			.NVIC_IRQChannelPreemptionPriority = PIOS_IRQ_PRIO_MID,
			.NVIC_IRQChannelSubPriority = 0,
			.NVIC_IRQChannelCmd = ENABLE,
		},
	},
	.exti = {
		.init = {
			.EXTI_Line = EXTI_Line15, // matches above GPIO pin
			.EXTI_Mode = EXTI_Mode_Interrupt,
			.EXTI_Trigger = EXTI_Trigger_Rising,
			.EXTI_LineCmd = ENABLE,
		},
	},
};

static const struct pios_mpu60x0_cfg pios_mpu6050_cfg = {
	.exti_cfg = &pios_exti_mpu6050_cfg,
	.default_samplerate = 500,
	.interrupt_cfg = PIOS_MPU60X0_INT_CLR_ANYRD,
	.interrupt_en = PIOS_MPU60X0_INTEN_DATA_RDY,
	.User_ctl = 0,
	.Pwr_mgmt_clk = PIOS_MPU60X0_PWRMGMT_PLL_Z_CLK,
	.default_filter = PIOS_MPU60X0_LOWPASS_256_HZ,
	.orientation = PIOS_MPU60X0_TOP_180DEG
};
#endif /* PIOS_INCLUDE_MPU6050 */

/**
 * Configuration for the MPU9150 chip
 */
#if defined(PIOS_INCLUDE_MPU9150)
#include "pios_mpu9150.h"
static const struct pios_exti_cfg pios_exti_mpu9150_cfg __exti_config = {
	.vector = PIOS_MPU9150_IRQHandler,
	.line = EXTI_Line15,
	.pin = {
		.gpio = GPIOA,
		.init = {
			.GPIO_Pin = GPIO_Pin_15,
			.GPIO_Speed = GPIO_Speed_50MHz,
			.GPIO_Mode = GPIO_Mode_IN,
			.GPIO_OType = GPIO_OType_OD,
			.GPIO_PuPd = GPIO_PuPd_NOPULL,
		},
	},
	.irq = {
		.init = {
			.NVIC_IRQChannel = EXTI15_10_IRQn,
			.NVIC_IRQChannelPreemptionPriority = PIOS_IRQ_PRIO_MID,
			.NVIC_IRQChannelSubPriority = 0,
			.NVIC_IRQChannelCmd = ENABLE,
		},
	},
	.exti = {
		.init = {
			.EXTI_Line = EXTI_Line15, // matches above GPIO pin
			.EXTI_Mode = EXTI_Mode_Interrupt,
			.EXTI_Trigger = EXTI_Trigger_Rising,
			.EXTI_LineCmd = ENABLE,
		},
	},
};

static struct pios_mpu60x0_cfg pios_mpu9150_cfg = {
	.exti_cfg = &pios_exti_mpu9150_cfg,
	.default_samplerate = 444,
	.interrupt_cfg = PIOS_MPU60X0_INT_CLR_ANYRD,
	.interrupt_en = PIOS_MPU60X0_INTEN_DATA_RDY,
	.User_ctl = 0,
	.Pwr_mgmt_clk = PIOS_MPU60X0_PWRMGMT_PLL_Z_CLK,
	.default_filter = PIOS_MPU60X0_LOWPASS_256_HZ,
	.orientation = PIOS_MPU60X0_TOP_180DEG,
	.use_internal_mag = true
};
#endif /* PIOS_INCLUDE_MPU9150 */

/**
+ * Configuration for the external HMC5883 chip
+ */
#if defined(PIOS_INCLUDE_HMC5883)
#include "pios_hmc5883_priv.h"
static const struct pios_hmc5883_cfg pios_hmc5883_external_cfg = {
       .exti_cfg            = NULL,
       .M_ODR               = PIOS_HMC5883_ODR_75,
       .Meas_Conf           = PIOS_HMC5883_MEASCONF_NORMAL,
       .Gain                = PIOS_HMC5883_GAIN_1_9,
       .Mode                = PIOS_HMC5883_MODE_SINGLE,
       .Default_Orientation = PIOS_HMC5883_TOP_0DEG,
};
#endif /* PIOS_INCLUDE_HMC5883 */

/* One slot per selectable receiver group.
 *  eg. PWM, PPM, GCS, SPEKTRUM1, SPEKTRUM2, SBUS
 * NOTE: No slot in this map for NONE.
 */
uintptr_t pios_rcvr_group_map[MANUALCONTROLSETTINGS_CHANNELGROUPS_NONE];

#define PIOS_COM_TELEM_RF_RX_BUF_LEN 512
#define PIOS_COM_TELEM_RF_TX_BUF_LEN 512

#define PIOS_COM_GPS_RX_BUF_LEN 32
#define PIOS_COM_GPS_TX_BUF_LEN 16

#define PIOS_COM_TELEM_USB_RX_BUF_LEN 65
#define PIOS_COM_TELEM_USB_TX_BUF_LEN 65

#define PIOS_COM_CAN_RX_BUF_LEN 256
#define PIOS_COM_CAN_TX_BUF_LEN 256

#define PIOS_COM_BRIDGE_RX_BUF_LEN 65
#define PIOS_COM_BRIDGE_TX_BUF_LEN 12

#define PIOS_COM_MAVLINK_TX_BUF_LEN 32
#define PIOS_COM_LIGHTTELEMETRY_TX_BUF_LEN 19 

#define PIOS_COM_HOTT_RX_BUF_LEN 16
#define PIOS_COM_HOTT_TX_BUF_LEN 16

#define PIOS_COM_FRSKYSENSORHUB_TX_BUF_LEN 128

#define PIOS_COM_FRSKYSPORT_TX_BUF_LEN 16
#define PIOS_COM_FRSKYSPORT_RX_BUF_LEN 16

#if defined(PIOS_INCLUDE_DEBUG_CONSOLE)
#define PIOS_COM_DEBUGCONSOLE_TX_BUF_LEN 40
uintptr_t pios_com_debug_id;
#endif	/* PIOS_INCLUDE_DEBUG_CONSOLE */

uintptr_t pios_com_aux_id;
uintptr_t pios_com_gps_id;
uintptr_t pios_com_telem_usb_id;
uintptr_t pios_com_telem_rf_id;
uintptr_t pios_com_vcp_id;
uintptr_t pios_com_bridge_id;
uintptr_t pios_com_mavlink_id;
uintptr_t pios_com_hott_id;
uintptr_t pios_com_frsky_sensor_hub_id;
uintptr_t pios_com_lighttelemetry_id; 
uintptr_t pios_com_can_id;
uintptr_t pios_com_frsky_sport_id;

uintptr_t pios_uavo_settings_fs_id;
uintptr_t pios_waypoints_settings_fs_id;

uintptr_t pios_internal_adc_id;

uintptr_t pios_can_id;

/*
 * Setup a com port based on the passed cfg, driver and buffer sizes. tx size of -1 make the port rx only
 */
#if defined(PIOS_INCLUDE_USART) && defined(PIOS_INCLUDE_COM)
static void PIOS_Board_configure_com (const struct pios_usart_cfg *usart_port_cfg, size_t rx_buf_len, size_t tx_buf_len,
		const struct pios_com_driver *com_driver, uintptr_t *pios_com_id)
{
	uintptr_t pios_usart_id;
	if (PIOS_USART_Init(&pios_usart_id, usart_port_cfg)) {
		PIOS_Assert(0);
	}

	uint8_t * rx_buffer;
	if (rx_buf_len > 0) {
		rx_buffer = (uint8_t *) PIOS_malloc(rx_buf_len);
		PIOS_Assert(rx_buffer);
	} else {
		rx_buffer = NULL;
	}

	uint8_t * tx_buffer;
	if (tx_buf_len > 0) {
		tx_buffer = (uint8_t *) PIOS_malloc(tx_buf_len);
		PIOS_Assert(tx_buffer);
	} else {
		tx_buffer = NULL;
	}

	if (PIOS_COM_Init(pios_com_id, com_driver, pios_usart_id,
				rx_buffer, rx_buf_len,
				tx_buffer, tx_buf_len)) {
		PIOS_Assert(0);
	}
}
#endif	/* PIOS_INCLUDE_USART && PIOS_INCLUDE_COM */

#ifdef PIOS_INCLUDE_DSM
static void PIOS_Board_configure_dsm(const struct pios_usart_cfg *pios_usart_dsm_cfg, const struct pios_dsm_cfg *pios_dsm_cfg,
		const struct pios_com_driver *pios_usart_com_driver,
		ManualControlSettingsChannelGroupsOptions channelgroup,uint8_t *bind)
{
	uintptr_t pios_usart_dsm_id;
	if (PIOS_USART_Init(&pios_usart_dsm_id, pios_usart_dsm_cfg)) {
		PIOS_Assert(0);
	}

	uintptr_t pios_dsm_id;
	if (PIOS_DSM_Init(&pios_dsm_id, pios_dsm_cfg, pios_usart_com_driver,
			pios_usart_dsm_id, *bind)) {
		PIOS_Assert(0);
	}

	uintptr_t pios_dsm_rcvr_id;
	if (PIOS_RCVR_Init(&pios_dsm_rcvr_id, &pios_dsm_rcvr_driver, pios_dsm_id)) {
		PIOS_Assert(0);
	}
	pios_rcvr_group_map[channelgroup] = pios_dsm_rcvr_id;
}
#endif

#ifdef PIOS_INCLUDE_HSUM
static void PIOS_Board_configure_hsum(const struct pios_usart_cfg *pios_usart_hsum_cfg,
		const struct pios_com_driver *pios_usart_com_driver,enum pios_hsum_proto *proto,
		ManualControlSettingsChannelGroupsOptions channelgroup)
{
	uintptr_t pios_usart_hsum_id;
	if (PIOS_USART_Init(&pios_usart_hsum_id, pios_usart_hsum_cfg)) {
		PIOS_Assert(0);
	}
	
	uintptr_t pios_hsum_id;
	if (PIOS_HSUM_Init(&pios_hsum_id, pios_usart_com_driver,
			  pios_usart_hsum_id, *proto)) {
		PIOS_Assert(0);
	}
	
	uintptr_t pios_hsum_rcvr_id;
	if (PIOS_RCVR_Init(&pios_hsum_rcvr_id, &pios_hsum_rcvr_driver, pios_hsum_id)) {
		PIOS_Assert(0);
	}
	pios_rcvr_group_map[channelgroup] = pios_hsum_rcvr_id;
}
#endif

/**
 * Indicate a target-specific error code when a component fails to initialize
 * 1 pulse - MPU9150 - no irq
 * 2 pulses - MPU9150 - failed configuration or task starting
 * 3 pulses - internal I2C bus locked
 * 4 pulses - external I2C bus locked
 * 5 pulses - flash
 * 6 pulses - CAN
 * 11 pulses - external HMC5883 failed
 */
void panic(int32_t code) {
	while(1){
		for (int32_t i = 0; i < code; i++) {
			PIOS_WDG_Clear();
			PIOS_LED_Toggle(PIOS_LED_ALARM);
			PIOS_DELAY_WaitmS(200);
			PIOS_WDG_Clear();
			PIOS_LED_Toggle(PIOS_LED_ALARM);
			PIOS_DELAY_WaitmS(200);
		}
		PIOS_WDG_Clear();
		PIOS_DELAY_WaitmS(200);
		PIOS_WDG_Clear();
		PIOS_DELAY_WaitmS(200);
		PIOS_WDG_Clear();
		PIOS_DELAY_WaitmS(100);
	}
}

/**
 * PIOS_Board_Init()
 * initializes all the core subsystems on this specific hardware
 * called from System/openpilot.c
 */

#include <pios_board_info.h>

void PIOS_Board_Init(void) {

	/* Delay system */
	PIOS_DELAY_Init();

	const struct pios_board_info * bdinfo = &pios_board_info_blob;

#if defined(PIOS_INCLUDE_LED)
	const struct pios_led_cfg * led_cfg = PIOS_BOARD_HW_DEFS_GetLedCfg(bdinfo->board_rev);
	PIOS_Assert(led_cfg);
	PIOS_LED_Init(led_cfg);
#endif	/* PIOS_INCLUDE_LED */

#if defined(PIOS_INCLUDE_I2C)
	if (PIOS_I2C_Init(&pios_i2c_internal_id, &pios_i2c_internal_cfg)) {
		PIOS_DEBUG_Assert(0);
	}
	if (PIOS_I2C_CheckClear(pios_i2c_internal_id) != 0)
		panic(3);
#endif

#if defined(PIOS_INCLUDE_CAN)
	if (PIOS_CAN_Init(&pios_can_id, &pios_can_cfg) != 0)
		panic(6);

	uint8_t * rx_buffer = (uint8_t *) PIOS_malloc(PIOS_COM_CAN_RX_BUF_LEN);
	uint8_t * tx_buffer = (uint8_t *) PIOS_malloc(PIOS_COM_CAN_TX_BUF_LEN);
	PIOS_Assert(rx_buffer);
	PIOS_Assert(tx_buffer);
	if (PIOS_COM_Init(&pios_com_can_id, &pios_can_com_driver, pios_can_id,
	                  rx_buffer, PIOS_COM_CAN_RX_BUF_LEN,
	                  tx_buffer, PIOS_COM_CAN_TX_BUF_LEN))
		panic(6);

	pios_com_bridge_id = pios_com_can_id;
#endif

#if defined(PIOS_INCLUDE_FLASH)
	/* Inititialize all flash drivers */
	if (PIOS_Flash_Internal_Init(&pios_internal_flash_id, &flash_internal_cfg) != 0)
		panic(5);

	/* Register the partition table */
	const struct pios_flash_partition * flash_partition_table;
	uint32_t num_partitions;
	flash_partition_table = PIOS_BOARD_HW_DEFS_GetPartitionTable(bdinfo->board_rev, &num_partitions);
	PIOS_FLASH_register_partition_table(flash_partition_table, num_partitions);

	/* Mount all filesystems */
	if (PIOS_FLASHFS_Logfs_Init(&pios_uavo_settings_fs_id, &flashfs_internal_settings_cfg, FLASH_PARTITION_LABEL_SETTINGS) != 0)
		panic(5);
	if (PIOS_FLASHFS_Logfs_Init(&pios_waypoints_settings_fs_id, &flashfs_internal_waypoints_cfg, FLASH_PARTITION_LABEL_WAYPOINTS) != 0)
		panic(5);

#if defined(ERASE_FLASH)
	PIOS_FLASHFS_Format(pios_uavo_settings_fs_id);
#endif

#endif	/* PIOS_INCLUDE_FLASH */

	/* Initialize the task monitor library */
	TaskMonitorInitialize();

	/* Initialize UAVObject libraries */
	EventDispatcherInitialize();
	UAVObjInitialize();

	/* Initialize the alarms library */
	AlarmsInitialize();

	HwSparkyInitialize();
	ModuleSettingsInitialize();

#if defined(PIOS_INCLUDE_RTC)
	/* Initialize the real-time clock and its associated tick */
	PIOS_RTC_Init(&pios_rtc_main_cfg);
#endif

	/* Initialize watchdog as early as possible to catch faults during init
	 * but do it only if there is no debugger connected
	 */
	if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) == 0) {
		PIOS_WDG_Init();
	}

	/* Set up pulse timers */
	//inputs

	//outputs
	PIOS_TIM_InitClock(&tim_1_cfg);
	PIOS_TIM_InitClock(&tim_2_cfg);
	PIOS_TIM_InitClock(&tim_3_cfg);
	PIOS_TIM_InitClock(&tim_15_cfg);
	PIOS_TIM_InitClock(&tim_16_cfg);
	PIOS_TIM_InitClock(&tim_17_cfg);

	/* IAP System Setup */
	PIOS_IAP_Init();
	uint16_t boot_count = PIOS_IAP_ReadBootCount();
	if (boot_count < 3) {
		PIOS_IAP_WriteBootCount(++boot_count);
		AlarmsClear(SYSTEMALARMS_ALARM_BOOTFAULT);
	} else {
		/* Too many failed boot attempts, force hw config to defaults */
		HwSparkySetDefaults(HwSparkyHandle(), 0);
		ModuleSettingsSetDefaults(ModuleSettingsHandle(),0);
		AlarmsSet(SYSTEMALARMS_ALARM_BOOTFAULT, SYSTEMALARMS_ALARM_CRITICAL);
	}

#if defined(PIOS_INCLUDE_USB)
	/* Initialize board specific USB data */
	PIOS_USB_BOARD_DATA_Init();

	/* Flags to determine if various USB interfaces are advertised */
	bool usb_hid_present = false;

#if defined(PIOS_INCLUDE_USB_CDC)
	bool usb_cdc_present = false;
	if (PIOS_USB_DESC_HID_CDC_Init()) {
		PIOS_Assert(0);
	}
	usb_hid_present = true;
	usb_cdc_present = true;
#else
	if (PIOS_USB_DESC_HID_ONLY_Init()) {
		PIOS_Assert(0);
	}
	usb_hid_present = true;
#endif

	uintptr_t pios_usb_id;
	PIOS_USB_Init(&pios_usb_id, PIOS_BOARD_HW_DEFS_GetUsbCfg(bdinfo->board_rev));

#if defined(PIOS_INCLUDE_USB_CDC)

	uint8_t hw_usb_vcpport;
	/* Configure the USB VCP port */
	HwSparkyUSB_VCPPortGet(&hw_usb_vcpport);

	if (!usb_cdc_present) {
		/* Force VCP port function to disabled if we haven't advertised VCP in our USB descriptor */
		hw_usb_vcpport = HWSPARKY_USB_VCPPORT_DISABLED;
	}

	switch (hw_usb_vcpport) {
	case HWSPARKY_USB_VCPPORT_DISABLED:
		break;
	case HWSPARKY_USB_VCPPORT_USBTELEMETRY:
#if defined(PIOS_INCLUDE_COM)
		{
			uintptr_t pios_usb_cdc_id;
			if (PIOS_USB_CDC_Init(&pios_usb_cdc_id, &pios_usb_cdc_cfg, pios_usb_id)) {
				PIOS_Assert(0);
			}
			uint8_t * rx_buffer = (uint8_t *) PIOS_malloc(PIOS_COM_TELEM_USB_RX_BUF_LEN);
			uint8_t * tx_buffer = (uint8_t *) PIOS_malloc(PIOS_COM_TELEM_USB_TX_BUF_LEN);
			PIOS_Assert(rx_buffer);
			PIOS_Assert(tx_buffer);
			if (PIOS_COM_Init(&pios_com_telem_usb_id, &pios_usb_cdc_com_driver, pios_usb_cdc_id,
						rx_buffer, PIOS_COM_TELEM_USB_RX_BUF_LEN,
						tx_buffer, PIOS_COM_TELEM_USB_TX_BUF_LEN)) {
				PIOS_Assert(0);
			}
		}
#endif	/* PIOS_INCLUDE_COM */
		break;
	case HWSPARKY_USB_VCPPORT_COMBRIDGE:
#if defined(PIOS_INCLUDE_COM)
		{
			uintptr_t pios_usb_cdc_id;
			if (PIOS_USB_CDC_Init(&pios_usb_cdc_id, &pios_usb_cdc_cfg, pios_usb_id)) {
				PIOS_Assert(0);
			}
			uint8_t * rx_buffer = (uint8_t *) PIOS_malloc(PIOS_COM_BRIDGE_RX_BUF_LEN);
			uint8_t * tx_buffer = (uint8_t *) PIOS_malloc(PIOS_COM_BRIDGE_TX_BUF_LEN);
			PIOS_Assert(rx_buffer);
			PIOS_Assert(tx_buffer);
			if (PIOS_COM_Init(&pios_com_vcp_id, &pios_usb_cdc_com_driver, pios_usb_cdc_id,
						rx_buffer, PIOS_COM_BRIDGE_RX_BUF_LEN,
						tx_buffer, PIOS_COM_BRIDGE_TX_BUF_LEN)) {
				PIOS_Assert(0);
			}
		}
#endif	/* PIOS_INCLUDE_COM */
		break;
	case HWSPARKY_USB_VCPPORT_DEBUGCONSOLE:
#if defined(PIOS_INCLUDE_COM)
#if defined(PIOS_INCLUDE_DEBUG_CONSOLE)
		{
			uintptr_t pios_usb_cdc_id;
			if (PIOS_USB_CDC_Init(&pios_usb_cdc_id, &pios_usb_cdc_cfg, pios_usb_id)) {
				PIOS_Assert(0);
			}
			uint8_t * tx_buffer = (uint8_t *) PIOS_malloc(PIOS_COM_DEBUGCONSOLE_TX_BUF_LEN);
			PIOS_Assert(tx_buffer);
			if (PIOS_COM_Init(&pios_com_debug_id, &pios_usb_cdc_com_driver, pios_usb_cdc_id,
						NULL, 0,
						tx_buffer, PIOS_COM_DEBUGCONSOLE_TX_BUF_LEN)) {
				PIOS_Assert(0);
			}
		}
#endif	/* PIOS_INCLUDE_DEBUG_CONSOLE */
#endif	/* PIOS_INCLUDE_COM */

		break;
	}
#endif	/* PIOS_INCLUDE_USB_CDC */

#if defined(PIOS_INCLUDE_USB_HID)
	/* Configure the usb HID port */
	uint8_t hw_usb_hidport;
	HwSparkyUSB_HIDPortGet(&hw_usb_hidport);

	if (!usb_hid_present) {
		/* Force HID port function to disabled if we haven't advertised HID in our USB descriptor */
		hw_usb_hidport = HWSPARKY_USB_HIDPORT_DISABLED;
	}

	switch (hw_usb_hidport) {
	case HWSPARKY_USB_HIDPORT_DISABLED:
		break;
	case HWSPARKY_USB_HIDPORT_USBTELEMETRY:
#if defined(PIOS_INCLUDE_COM)
		{
			uintptr_t pios_usb_hid_id;
			if (PIOS_USB_HID_Init(&pios_usb_hid_id, &pios_usb_hid_cfg, pios_usb_id)) {
				PIOS_Assert(0);
			}
			uint8_t * rx_buffer = (uint8_t *)PIOS_malloc(PIOS_COM_TELEM_USB_RX_BUF_LEN);
			uint8_t * tx_buffer = (uint8_t *)PIOS_malloc(PIOS_COM_TELEM_USB_TX_BUF_LEN);
			PIOS_Assert(rx_buffer);
			PIOS_Assert(tx_buffer);
			if (PIOS_COM_Init(&pios_com_telem_usb_id, &pios_usb_hid_com_driver, pios_usb_hid_id,
						rx_buffer, PIOS_COM_TELEM_USB_RX_BUF_LEN,
						tx_buffer, PIOS_COM_TELEM_USB_TX_BUF_LEN)) {
				PIOS_Assert(0);
			}
		}
#endif	/* PIOS_INCLUDE_COM */
		break;
	}

#endif	/* PIOS_INCLUDE_USB_HID */
#endif	/* PIOS_INCLUDE_USB */

	/* Configure the IO ports */
	uint8_t hw_DSMxBind;
	HwSparkyDSMxBindGet(&hw_DSMxBind);

	/* UART1 Port */
	uint8_t hw_flexi;
	HwSparkyFlexiPortGet(&hw_flexi);
	switch (hw_flexi) {
	case HWSPARKY_FLEXIPORT_DISABLED:
		break;
	case HWSPARKY_FLEXIPORT_TELEMETRY:
#if defined(PIOS_INCLUDE_TELEMETRY_RF) && defined(PIOS_INCLUDE_USART) && defined(PIOS_INCLUDE_COM)
		PIOS_Board_configure_com(&pios_flexi_usart_cfg, PIOS_COM_TELEM_RF_RX_BUF_LEN, PIOS_COM_TELEM_RF_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_telem_rf_id);
#endif /* PIOS_INCLUDE_TELEMETRY_RF */
		break;
	case HWSPARKY_FLEXIPORT_GPS:
#if defined(PIOS_INCLUDE_GPS) && defined(PIOS_INCLUDE_USART) && defined(PIOS_INCLUDE_COM)
		PIOS_Board_configure_com(&pios_flexi_usart_cfg, PIOS_COM_GPS_RX_BUF_LEN, PIOS_COM_GPS_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_gps_id);
#endif
		break;
       case HWSPARKY_FLEXIPORT_I2C:
#if defined(PIOS_INCLUDE_I2C)
			if (PIOS_I2C_Init(&pios_i2c_flexi_id, &pios_i2c_flexi_cfg)) {
				PIOS_Assert(0);
			}

			if (PIOS_I2C_CheckClear(pios_i2c_flexi_id) != 0)
				panic(11);
#endif /* PIOS_INCLUDE_I2C */
               break;
	case HWSPARKY_FLEXIPORT_SBUS:
#if defined(PIOS_INCLUDE_SBUS) && defined(PIOS_INCLUDE_USART)
		{
			uintptr_t pios_usart_sbus_id;
			if (PIOS_USART_Init(&pios_usart_sbus_id, &pios_flexi_sbus_cfg)) {
				PIOS_Assert(0);
			}
			uintptr_t pios_sbus_id;
			if (PIOS_SBus_Init(&pios_sbus_id, &pios_flexi_sbus_aux_cfg, &pios_usart_com_driver, pios_usart_sbus_id)) {
				PIOS_Assert(0);
			}
			uintptr_t pios_sbus_rcvr_id;
			if (PIOS_RCVR_Init(&pios_sbus_rcvr_id, &pios_sbus_rcvr_driver, pios_sbus_id)) {
				PIOS_Assert(0);
			}
			pios_rcvr_group_map[MANUALCONTROLSETTINGS_CHANNELGROUPS_SBUS] = pios_sbus_rcvr_id;
		}
#endif	/* PIOS_INCLUDE_SBUS */
		break;
	case HWSPARKY_FLEXIPORT_DSM:
#if defined(PIOS_INCLUDE_DSM)
		{
			PIOS_Board_configure_dsm(&pios_flexi_dsm_hsum_cfg, &pios_flexi_dsm_aux_cfg, &pios_usart_com_driver,
				MANUALCONTROLSETTINGS_CHANNELGROUPS_DSM, &hw_DSMxBind);
		}
#endif	/* PIOS_INCLUDE_DSM */
		break;
	case HWSPARKY_FLEXIPORT_DEBUGCONSOLE:
#if defined(PIOS_INCLUDE_DEBUG_CONSOLE) && defined(PIOS_INCLUDE_USART) && defined(PIOS_INCLUDE_COM)
		PIOS_Board_configure_com(&pios_flexi_usart_cfg, 0, PIOS_COM_DEBUGCONSOLE_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_aux_id);
#endif	/* PIOS_INCLUDE_DEBUG_CONSOLE */
		break;
	case HWSPARKY_FLEXIPORT_COMBRIDGE:
#if defined(PIOS_INCLUDE_USART) && defined(PIOS_INCLUDE_COM)
		PIOS_Board_configure_com(&pios_flexi_usart_cfg, PIOS_COM_BRIDGE_RX_BUF_LEN, PIOS_COM_BRIDGE_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_bridge_id);
#endif
		break;
	case HWSPARKY_FLEXIPORT_MAVLINKTX:
#if defined(PIOS_INCLUDE_MAVLINK)
		PIOS_Board_configure_com(&pios_flexi_usart_cfg, 0, PIOS_COM_MAVLINK_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_mavlink_id);
#endif  /* PIOS_INCLUDE_MAVLINK */
		break;
	case HWSPARKY_FLEXIPORT_MAVLINKTX_GPS_RX:
#if defined(PIOS_INCLUDE_GPS)
#if defined(PIOS_INCLUDE_MAVLINK)
		PIOS_Board_configure_com(&pios_flexi_usart_cfg, PIOS_COM_GPS_RX_BUF_LEN, PIOS_COM_MAVLINK_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_gps_id);
		pios_com_mavlink_id = pios_com_gps_id;
#endif  /* PIOS_INCLUDE_MAVLINK */
#endif  /* PIOS_INCLUDE_GPS */
		break;
	case HWSPARKY_FLEXIPORT_HOTTTELEMETRY:
#if defined(PIOS_INCLUDE_HOTT) && defined(PIOS_INCLUDE_USART) && defined(PIOS_INCLUDE_COM)
		PIOS_Board_configure_com(&pios_flexi_usart_cfg, PIOS_COM_HOTT_RX_BUF_LEN, PIOS_COM_HOTT_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_hott_id);
#endif /* PIOS_INCLUDE_HOTT */
		break;
	case HWSPARKY_FLEXIPORT_FRSKYSENSORHUB:
#if defined(PIOS_INCLUDE_FRSKY_SENSOR_HUB) && defined(PIOS_INCLUDE_USART) && defined(PIOS_INCLUDE_COM)
		PIOS_Board_configure_com(&pios_flexi_usart_sport_cfg, 0, PIOS_COM_FRSKYSENSORHUB_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_frsky_sensor_hub_id);
#endif /* PIOS_INCLUDE_FRSKY_SENSOR_HUB */
		break;
	case HWSPARKY_FLEXIPORT_LIGHTTELEMETRYTX:
#if defined(PIOS_INCLUDE_LIGHTTELEMETRY)
	PIOS_Board_configure_com(&pios_flexi_usart_cfg, 0, PIOS_COM_LIGHTTELEMETRY_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_lighttelemetry_id);
#endif
		break;
	case HWSPARKY_FLEXIPORT_FRSKYSPORTTELEMETRY:
#if defined(PIOS_INCLUDE_FRSKY_SPORT_TELEMETRY)
		PIOS_Board_configure_com(&pios_flexi_usart_sport_cfg, PIOS_COM_FRSKYSPORT_RX_BUF_LEN, PIOS_COM_FRSKYSPORT_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_frsky_sport_id);
#endif /* PIOS_INCLUDE_FRSKY_SPORT_TELEMETRY */
		break;
	}

	/* UART3 Port */
	uint8_t hw_main;
	HwSparkyMainPortGet(&hw_main);
	switch (hw_main) {
	case HWSPARKY_MAINPORT_DISABLED:
		break;
	case HWSPARKY_MAINPORT_TELEMETRY:
#if defined(PIOS_INCLUDE_TELEMETRY_RF) && defined(PIOS_INCLUDE_USART) && defined(PIOS_INCLUDE_COM)
		PIOS_Board_configure_com(&pios_main_usart_cfg, PIOS_COM_TELEM_RF_RX_BUF_LEN, PIOS_COM_TELEM_RF_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_telem_rf_id);
#endif /* PIOS_INCLUDE_TELEMETRY_RF */
		break;
	case HWSPARKY_MAINPORT_GPS:
#if defined(PIOS_INCLUDE_GPS) && defined(PIOS_INCLUDE_USART) && defined(PIOS_INCLUDE_COM)
		PIOS_Board_configure_com(&pios_main_usart_cfg, PIOS_COM_GPS_RX_BUF_LEN, PIOS_COM_GPS_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_gps_id);
#endif
		break;
	case HWSPARKY_MAINPORT_SBUS:
#if defined(PIOS_INCLUDE_SBUS) && defined(PIOS_INCLUDE_USART)
		{
			uintptr_t pios_usart_sbus_id;
			if (PIOS_USART_Init(&pios_usart_sbus_id, &pios_main_sbus_cfg)) {
				PIOS_Assert(0);
			}
			uintptr_t pios_sbus_id;
			if (PIOS_SBus_Init(&pios_sbus_id, &pios_main_sbus_aux_cfg, &pios_usart_com_driver, pios_usart_sbus_id)) {
				PIOS_Assert(0);
			}
			uintptr_t pios_sbus_rcvr_id;
			if (PIOS_RCVR_Init(&pios_sbus_rcvr_id, &pios_sbus_rcvr_driver, pios_sbus_id)) {
				PIOS_Assert(0);
			}
			pios_rcvr_group_map[MANUALCONTROLSETTINGS_CHANNELGROUPS_SBUS] = pios_sbus_rcvr_id;
		}
#endif	/* PIOS_INCLUDE_SBUS */
		break;
	case HWSPARKY_MAINPORT_DSM:
#if defined(PIOS_INCLUDE_DSM)
		{
			PIOS_Board_configure_dsm(&pios_main_dsm_hsum_cfg, &pios_main_dsm_aux_cfg, &pios_usart_com_driver,
				MANUALCONTROLSETTINGS_CHANNELGROUPS_DSM, &hw_DSMxBind);
		}
#endif	/* PIOS_INCLUDE_DSM */
		break;
	case HWSPARKY_MAINPORT_DEBUGCONSOLE:
#if defined(PIOS_INCLUDE_DEBUG_CONSOLE) && defined(PIOS_INCLUDE_USART) && defined(PIOS_INCLUDE_COM)
		PIOS_Board_configure_com(&pios_main_usart_cfg, 0, PIOS_COM_DEBUGCONSOLE_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_aux_id);
#endif	/* PIOS_INCLUDE_DEBUG_CONSOLE */
		break;
	case HWSPARKY_MAINPORT_COMBRIDGE:
#if defined(PIOS_INCLUDE_USART) && defined(PIOS_INCLUDE_COM)
		PIOS_Board_configure_com(&pios_main_usart_cfg, PIOS_COM_BRIDGE_RX_BUF_LEN, PIOS_COM_BRIDGE_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_bridge_id);
#endif
		break;
	case HWSPARKY_MAINPORT_MAVLINKTX:
#if defined(PIOS_INCLUDE_MAVLINK)
		PIOS_Board_configure_com(&pios_main_usart_cfg, 0, PIOS_COM_MAVLINK_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_mavlink_id);
#endif  /* PIOS_INCLUDE_MAVLINK */
		break;
	case HWSPARKY_MAINPORT_MAVLINKTX_GPS_RX:
#if defined(PIOS_INCLUDE_GPS)
#if defined(PIOS_INCLUDE_MAVLINK)
		PIOS_Board_configure_com(&pios_main_usart_cfg, PIOS_COM_GPS_RX_BUF_LEN, PIOS_COM_MAVLINK_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_gps_id);
		pios_com_mavlink_id = pios_com_gps_id;
#endif  /* PIOS_INCLUDE_MAVLINK */
#endif  /* PIOS_INCLUDE_GPS */
		break;
	case HWSPARKY_MAINPORT_HOTTTELEMETRY:
#if defined(PIOS_INCLUDE_HOTT) && defined(PIOS_INCLUDE_USART) && defined(PIOS_INCLUDE_COM)
		PIOS_Board_configure_com(&pios_main_usart_cfg, PIOS_COM_HOTT_RX_BUF_LEN, PIOS_COM_HOTT_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_hott_id);
#endif /* PIOS_INCLUDE_HOTT */
		break;
	case HWSPARKY_MAINPORT_FRSKYSENSORHUB:
#if defined(PIOS_INCLUDE_FRSKY_SENSOR_HUB) && defined(PIOS_INCLUDE_USART) && defined(PIOS_INCLUDE_COM)
		PIOS_Board_configure_com(&pios_main_usart_sport_cfg, 0, PIOS_COM_FRSKYSENSORHUB_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_frsky_sensor_hub_id);
#endif /* PIOS_INCLUDE_FRSKY_SENSOR_HUB */
		break;
	case HWSPARKY_MAINPORT_LIGHTTELEMETRYTX:
#if defined(PIOS_INCLUDE_LIGHTTELEMETRY)
	PIOS_Board_configure_com(&pios_main_usart_cfg, 0, PIOS_COM_LIGHTTELEMETRY_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_lighttelemetry_id);
#endif /* PIOS_INCLUDE_LIGHTTELEMETRY */
		break;
	case HWSPARKY_MAINPORT_FRSKYSPORTTELEMETRY:
#if defined(PIOS_INCLUDE_FRSKY_SPORT_TELEMETRY)
		PIOS_Board_configure_com(&pios_main_usart_sport_cfg, PIOS_COM_FRSKYSPORT_RX_BUF_LEN, PIOS_COM_FRSKYSPORT_TX_BUF_LEN, &pios_usart_com_driver, &pios_com_frsky_sport_id);
#endif /* PIOS_INCLUDE_FRSKY_SPORT_TELEMETRY */
		break;
	}

	/* Configure the rcvr port */
	uint8_t hw_rcvrport;
	HwSparkyRcvrPortGet(&hw_rcvrport);

	switch (hw_rcvrport) {
	case HWSPARKY_RCVRPORT_DISABLED:
		break;
	case HWSPARKY_RCVRPORT_PPM:
#if defined(PIOS_INCLUDE_PPM)
		{
			uintptr_t pios_ppm_id;
			PIOS_PPM_Init(&pios_ppm_id, &pios_ppm_cfg);

			uintptr_t pios_ppm_rcvr_id;
			if (PIOS_RCVR_Init(&pios_ppm_rcvr_id, &pios_ppm_rcvr_driver, pios_ppm_id)) {
				PIOS_Assert(0);
			}
			pios_rcvr_group_map[MANUALCONTROLSETTINGS_CHANNELGROUPS_PPM] = pios_ppm_rcvr_id;
		}
#endif	/* PIOS_INCLUDE_PPM */
		break;
	case HWSPARKY_RCVRPORT_DSM:
#if defined(PIOS_INCLUDE_DSM)
		{
			PIOS_Board_configure_dsm(&pios_rcvr_dsm_hsum_cfg, &pios_rcvr_dsm_aux_cfg, &pios_usart_com_driver,
				MANUALCONTROLSETTINGS_CHANNELGROUPS_DSM, &hw_DSMxBind);
		}
#endif	/* PIOS_INCLUDE_DSM */
		break;
	case HWSPARKY_RCVRPORT_HOTTSUMD:
	case HWSPARKY_RCVRPORT_HOTTSUMH:
#if defined(PIOS_INCLUDE_HSUM)
		{
			enum pios_hsum_proto proto;
			switch (hw_rcvrport) {
			case HWSPARKY_RCVRPORT_HOTTSUMD:
				proto = PIOS_HSUM_PROTO_SUMD;
				break;
			case HWSPARKY_RCVRPORT_HOTTSUMH:
				proto = PIOS_HSUM_PROTO_SUMH;
				break;
			default:
				PIOS_Assert(0);
				break;
			}
			PIOS_Board_configure_hsum(&pios_rcvr_dsm_hsum_cfg, &pios_usart_com_driver,
				&proto, MANUALCONTROLSETTINGS_CHANNELGROUPS_HOTTSUM);
		}
#endif	/* PIOS_INCLUDE_HSUM */
		break;
	case HWSPARKY_RCVRPORT_SBUS:
#if defined(PIOS_INCLUDE_SBUS) && defined(PIOS_INCLUDE_USART)
		{
			uintptr_t pios_usart_sbus_id;
			if (PIOS_USART_Init(&pios_usart_sbus_id, &pios_rcvr_sbus_cfg)) {
				PIOS_Assert(0);
			}
			uintptr_t pios_sbus_id;
			if (PIOS_SBus_Init(&pios_sbus_id, &pios_rcvr_sbus_aux_cfg, &pios_usart_com_driver, pios_usart_sbus_id)) {
				PIOS_Assert(0);
			}
			uintptr_t pios_sbus_rcvr_id;
			if (PIOS_RCVR_Init(&pios_sbus_rcvr_id, &pios_sbus_rcvr_driver, pios_sbus_id)) {
				PIOS_Assert(0);
			}
			pios_rcvr_group_map[MANUALCONTROLSETTINGS_CHANNELGROUPS_SBUS] = pios_sbus_rcvr_id;
		}
#endif	/* PIOS_INCLUDE_SBUS */
		break;		break;
	}


#if defined(PIOS_INCLUDE_GCSRCVR)
	GCSReceiverInitialize();
	uintptr_t pios_gcsrcvr_id;
	PIOS_GCSRCVR_Init(&pios_gcsrcvr_id);
	uintptr_t pios_gcsrcvr_rcvr_id;
	if (PIOS_RCVR_Init(&pios_gcsrcvr_rcvr_id, &pios_gcsrcvr_rcvr_driver, pios_gcsrcvr_id)) {
		PIOS_Assert(0);
	}
	pios_rcvr_group_map[MANUALCONTROLSETTINGS_CHANNELGROUPS_GCS] = pios_gcsrcvr_rcvr_id;
#endif	/* PIOS_INCLUDE_GCSRCVR */

	uint8_t hw_outport;
	uint8_t number_of_pwm_outputs;
	uint8_t number_of_adc_ports;
	bool use_pwm_in;
	HwSparkyOutPortGet(&hw_outport);
	switch (hw_outport) {
	case HWSPARKY_OUTPORT_PWM10:
		number_of_pwm_outputs = 10;
		number_of_adc_ports = 0;
		use_pwm_in = false;
		break;
	case HWSPARKY_OUTPORT_PWM82ADC:
		number_of_pwm_outputs = 8;
		number_of_adc_ports = 2;
		use_pwm_in = false;
		break;
	case HWSPARKY_OUTPORT_PWM73ADC:
		number_of_pwm_outputs = 7;
		number_of_adc_ports = 3;
		use_pwm_in = false;
		break;
	case HWSPARKY_OUTPORT_PWM9PWM_IN:
		number_of_pwm_outputs = 9;
		use_pwm_in = true;
		number_of_adc_ports = 0;
		break;
	case HWSPARKY_OUTPORT_PWM7PWM_IN2ADC:
		number_of_pwm_outputs = 7;
		use_pwm_in = true;
		number_of_adc_ports = 2;
		break;
	default:
		PIOS_Assert(0);
		break;
	}
#ifndef PIOS_DEBUG_ENABLE_DEBUG_PINS
#ifdef PIOS_INCLUDE_SERVO
	pios_servo_cfg.num_channels = number_of_pwm_outputs;
	PIOS_Servo_Init(&pios_servo_cfg);
#endif
#else
	PIOS_DEBUG_Init(&pios_tim_servo_all_channels, NELEMENTS(pios_tim_servo_all_channels));
#endif

#if defined(PIOS_INCLUDE_ADC)
	if(number_of_adc_ports > 0) {
		internal_adc_cfg.number_of_used_pins = number_of_adc_ports;
		uint32_t internal_adc_id;
		if(PIOS_INTERNAL_ADC_Init(&internal_adc_id, &internal_adc_cfg) < 0)
			PIOS_Assert(0);
		PIOS_ADC_Init(&pios_internal_adc_id, &pios_internal_adc_driver, internal_adc_id);
	}
#endif /* PIOS_INCLUDE_ADC */
#if defined(PIOS_INCLUDE_PWM)
	if (use_pwm_in > 0) {
		if (number_of_adc_ports > 0)
			pios_pwm_cfg.channels = &pios_tim_rcvrport_pwm[1];
		uintptr_t pios_pwm_id;
		PIOS_PWM_Init(&pios_pwm_id, &pios_pwm_cfg);

		uintptr_t pios_pwm_rcvr_id;
		if (PIOS_RCVR_Init(&pios_pwm_rcvr_id, &pios_pwm_rcvr_driver, pios_pwm_id)) {
			PIOS_Assert(0);
		}
		pios_rcvr_group_map[MANUALCONTROLSETTINGS_CHANNELGROUPS_PWM] = pios_pwm_rcvr_id;
	}
#endif	/* PIOS_INCLUDE_PWM */
	PIOS_WDG_Clear();
	PIOS_DELAY_WaitmS(200);
	PIOS_WDG_Clear();

#if defined(PIOS_INCLUDE_MPU9150)
#if defined(PIOS_INCLUDE_MPU6050)
	// Enable autoprobing when both 6050 and 9050 compiled in
	bool mpu9150_found = false;
	if (PIOS_MPU9150_Probe(pios_i2c_internal_id, PIOS_MPU9150_I2C_ADD_A0_LOW) == 0) {
		mpu9150_found = true;
#else
	{
#endif /* PIOS_INCLUDE_MPU6050 */

		uint8_t Magnetometer;
		HwSparkyMagnetometerGet(&Magnetometer);

		if (Magnetometer == HWSPARKY_MAGNETOMETER_INTERNAL)
			pios_mpu9150_cfg.use_internal_mag = true;
		else
			pios_mpu9150_cfg.use_internal_mag = false;


		int retval;
		retval = PIOS_MPU9150_Init(pios_i2c_internal_id, PIOS_MPU9150_I2C_ADD_A0_LOW, &pios_mpu9150_cfg);
		if (retval == -10)
			panic(1); // indicate missing IRQ separately
		if (retval != 0)
			panic(2);

		// To be safe map from UAVO enum to driver enum
		uint8_t hw_gyro_range;
		HwSparkyGyroRangeGet(&hw_gyro_range);
		switch(hw_gyro_range) {
			case HWSPARKY_GYRORANGE_250:
				PIOS_MPU9150_SetGyroRange(PIOS_MPU60X0_SCALE_250_DEG);
				break;
			case HWSPARKY_GYRORANGE_500:
				PIOS_MPU9150_SetGyroRange(PIOS_MPU60X0_SCALE_500_DEG);
				break;
			case HWSPARKY_GYRORANGE_1000:
				PIOS_MPU9150_SetGyroRange(PIOS_MPU60X0_SCALE_1000_DEG);
				break;
			case HWSPARKY_GYRORANGE_2000:
				PIOS_MPU9150_SetGyroRange(PIOS_MPU60X0_SCALE_2000_DEG);
				break;
		}

		uint8_t hw_accel_range;
		HwSparkyAccelRangeGet(&hw_accel_range);
		switch(hw_accel_range) {
			case HWSPARKY_ACCELRANGE_2G:
				PIOS_MPU9150_SetAccelRange(PIOS_MPU60X0_ACCEL_2G);
				break;
			case HWSPARKY_ACCELRANGE_4G:
				PIOS_MPU9150_SetAccelRange(PIOS_MPU60X0_ACCEL_4G);
				break;
			case HWSPARKY_ACCELRANGE_8G:
				PIOS_MPU9150_SetAccelRange(PIOS_MPU60X0_ACCEL_8G);
				break;
			case HWSPARKY_ACCELRANGE_16G:
				PIOS_MPU9150_SetAccelRange(PIOS_MPU60X0_ACCEL_16G);
				break;
		}

		uint8_t hw_mpu9150_dlpf;
		HwSparkyMPU9150DLPFGet(&hw_mpu9150_dlpf);
		enum pios_mpu60x0_filter mpu9150_dlpf = \
		    (hw_mpu9150_dlpf == HWSPARKY_MPU9150DLPF_256) ? PIOS_MPU60X0_LOWPASS_256_HZ : \
		    (hw_mpu9150_dlpf == HWSPARKY_MPU9150DLPF_188) ? PIOS_MPU60X0_LOWPASS_188_HZ : \
		    (hw_mpu9150_dlpf == HWSPARKY_MPU9150DLPF_98) ? PIOS_MPU60X0_LOWPASS_98_HZ : \
		    (hw_mpu9150_dlpf == HWSPARKY_MPU9150DLPF_42) ? PIOS_MPU60X0_LOWPASS_42_HZ : \
		    (hw_mpu9150_dlpf == HWSPARKY_MPU9150DLPF_20) ? PIOS_MPU60X0_LOWPASS_20_HZ : \
		    (hw_mpu9150_dlpf == HWSPARKY_MPU9150DLPF_10) ? PIOS_MPU60X0_LOWPASS_10_HZ : \
		    (hw_mpu9150_dlpf == HWSPARKY_MPU9150DLPF_5) ? PIOS_MPU60X0_LOWPASS_5_HZ : \
		    pios_mpu9150_cfg.default_filter;
		PIOS_MPU9150_SetLPF(mpu9150_dlpf);

		uint8_t hw_mpu9150_samplerate;
		HwSparkyMPU9150RateGet(&hw_mpu9150_samplerate);
		uint16_t mpu9150_samplerate = \
		    (hw_mpu9150_samplerate == HWSPARKY_MPU9150RATE_200) ? 200 : \
		    (hw_mpu9150_samplerate == HWSPARKY_MPU9150RATE_333) ? 333 : \
		    (hw_mpu9150_samplerate == HWSPARKY_MPU9150RATE_500) ? 500 : \
		    (hw_mpu9150_samplerate == HWSPARKY_MPU9150RATE_666) ? 666 : \
		    (hw_mpu9150_samplerate == HWSPARKY_MPU9150RATE_1000) ? 1000 : \
		    (hw_mpu9150_samplerate == HWSPARKY_MPU9150RATE_2000) ? 2000 : \
		    (hw_mpu9150_samplerate == HWSPARKY_MPU9150RATE_4000) ? 4000 : \
		    (hw_mpu9150_samplerate == HWSPARKY_MPU9150RATE_8000) ? 8000 : \
		    pios_mpu9150_cfg.default_samplerate;
		PIOS_MPU9150_SetSampleRate(mpu9150_samplerate);	
	}

#endif /* PIOS_INCLUDE_MPU9150 */

#if defined(PIOS_INCLUDE_MPU6050)
#if defined(PIOS_INCLUDE_MPU9150)
	// MPU9150 looks like an MPU6050 _plus_ additional hardware.  So we cannot try and
	// probe if MPU9150 is found or we will find a duplicate
	if (mpu9150_found == false)
#endif /* PIOS_INCLUDE_MPU9150 */
	{
		if (PIOS_MPU6050_Init(pios_i2c_internal_id, PIOS_MPU6050_I2C_ADD_A0_LOW, &pios_mpu6050_cfg) != 0)
			panic(2);
		if (PIOS_MPU6050_Test() != 0)
			panic(2);

		// To be safe map from UAVO enum to driver enum
		uint8_t hw_gyro_range;
		HwSparkyGyroRangeGet(&hw_gyro_range);
		switch(hw_gyro_range) {
			case HWSPARKY_GYRORANGE_250:
				PIOS_MPU6050_SetGyroRange(PIOS_MPU60X0_SCALE_250_DEG);
				break;
			case HWSPARKY_GYRORANGE_500:
				PIOS_MPU6050_SetGyroRange(PIOS_MPU60X0_SCALE_500_DEG);
				break;
			case HWSPARKY_GYRORANGE_1000:
				PIOS_MPU6050_SetGyroRange(PIOS_MPU60X0_SCALE_1000_DEG);
				break;
			case HWSPARKY_GYRORANGE_2000:
				PIOS_MPU6050_SetGyroRange(PIOS_MPU60X0_SCALE_2000_DEG);
				break;
		}

		uint8_t hw_accel_range;
		HwSparkyAccelRangeGet(&hw_accel_range);
		switch(hw_accel_range) {
			case HWSPARKY_ACCELRANGE_2G:
				PIOS_MPU6050_SetAccelRange(PIOS_MPU60X0_ACCEL_2G);
				break;
			case HWSPARKY_ACCELRANGE_4G:
				PIOS_MPU6050_SetAccelRange(PIOS_MPU60X0_ACCEL_4G);
				break;
			case HWSPARKY_ACCELRANGE_8G:
				PIOS_MPU6050_SetAccelRange(PIOS_MPU60X0_ACCEL_8G);
				break;
			case HWSPARKY_ACCELRANGE_16G:
				PIOS_MPU6050_SetAccelRange(PIOS_MPU60X0_ACCEL_16G);
				break;
		}
	}

#endif /* PIOS_INCLUDE_MPU6050 */

	//I2C is slow, sensor init as well, reset watchdog to prevent reset here
	PIOS_WDG_Clear();

#if defined(PIOS_INCLUDE_HMC5883)
	{
		uint8_t Magnetometer;
		HwSparkyMagnetometerGet(&Magnetometer);

		if (Magnetometer == HWSPARKY_MAGNETOMETER_EXTERNALI2CFLEXIPORT)
		{
			// setup sensor orientation
			uint8_t ExtMagOrientation;
			HwSparkyExtMagOrientationGet(&ExtMagOrientation);

			if (Magnetometer == HWSPARKY_MAGNETOMETER_EXTERNALI2CFLEXIPORT) {
				// init sensor
				pios_mpu9150_cfg.use_internal_mag = false;

				//I2C is slow, sensor init as well, reset watchdog to prevent reset here
				PIOS_WDG_Clear();

				if (PIOS_HMC5883_Init(pios_i2c_flexi_id, &pios_hmc5883_external_cfg) != 0)
					panic(11);
				if (PIOS_HMC5883_Test() != 0)
					panic(11);
			}

			enum pios_hmc5883_orientation hmc5883_orientation = \
				(ExtMagOrientation == HWSPARKY_EXTMAGORIENTATION_TOP0DEGCW)      ? PIOS_HMC5883_TOP_0DEG      : \
				(ExtMagOrientation == HWSPARKY_EXTMAGORIENTATION_TOP90DEGCW)     ? PIOS_HMC5883_TOP_90DEG     : \
				(ExtMagOrientation == HWSPARKY_EXTMAGORIENTATION_TOP180DEGCW)    ? PIOS_HMC5883_TOP_180DEG    : \
				(ExtMagOrientation == HWSPARKY_EXTMAGORIENTATION_TOP270DEGCW)    ? PIOS_HMC5883_TOP_270DEG    : \
				(ExtMagOrientation == HWSPARKY_EXTMAGORIENTATION_BOTTOM0DEGCW)   ? PIOS_HMC5883_BOTTOM_0DEG   : \
				(ExtMagOrientation == HWSPARKY_EXTMAGORIENTATION_BOTTOM90DEGCW)  ? PIOS_HMC5883_BOTTOM_90DEG  : \
				(ExtMagOrientation == HWSPARKY_EXTMAGORIENTATION_BOTTOM180DEGCW) ? PIOS_HMC5883_BOTTOM_180DEG : \
				(ExtMagOrientation == HWSPARKY_EXTMAGORIENTATION_BOTTOM270DEGCW) ? PIOS_HMC5883_BOTTOM_270DEG : \
				pios_hmc5883_external_cfg.Default_Orientation;
			PIOS_HMC5883_SetOrientation(hmc5883_orientation);
		}
	}
#endif /* PIOS_INCLUDE_HMC5883 */

	//I2C is slow, sensor init as well, reset watchdog to prevent reset here
	PIOS_WDG_Clear();

#if defined(PIOS_INCLUDE_MS5611)
	PIOS_MS5611_Init(&pios_ms5611_cfg, pios_i2c_internal_id);
	if (PIOS_MS5611_Test() != 0)
		panic(4);
#endif

#if defined(PIOS_INCLUDE_GPIO)
	PIOS_GPIO_Init();
#endif

	/* Make sure we have at least one telemetry link configured or else fail initialization */
	PIOS_Assert(pios_com_telem_rf_id || pios_com_telem_usb_id);
}

/**
 * @}
 */
