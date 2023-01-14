Version -> BSW_V3.8.0
Commit SHA -> dc2b8458
## BSW_V3.8.0 (20220727)
### General
- add MCCM5A variants 401957 and 401959 [/intern]
- add SCR support [CAT210009-35] [/intern]
### Features
- added automatic processor counting for makefile (-j option) which automatically selects the max number of processors of the given computer [KSB-849]
- MCCM5A: add modulhardwarecode_motor_controller()-function for customer [CUS000001-657] [/intern]
- added the physical connector pin number/name to the MGW and CC16WP IO_DEF. This allows for a better pin selection inside the graphical programming, letting the user know which physical pin will be controlled when selecting the signal name. [KSB-1036]
- added the io_variables.c/.h files to src/ds/io directory for support of the VARAIBLE_IN and VARIABLE_OUT feature of the graphical programming [KSB-1027] [/intern] 
### Fixes
- updated DO_POWER pin name, visibility and description in IO_DEF for MGW module [KSB-1053]
- updated HW_TEST version because of HAWE related usercode_init fix regarding the adc_vrefh value, which was unnecessary because the HW_TEST is working with the raw digit values [KSB-1023] [/intern]
- fixed error in duty cycle and frequency measurement on frequencies above 3kHz, which caused spikes (positive and negative) during the measurement. [KSB-944] [KSB-1015] 
- fixed sleep mode behavior for all modules with LIN on board, by adapting the initialization of the LIN tranceiver when LIN is not used. [SUP-681] [KSB-941]
- fixed doxygen job for user_api which failed the pipeline [/intern]
- fixed bug which caused the wrong order for outgoing CAN messages under some specific circumstances, caused by the usage of two TX message boxes. [KSB-958] [FAEEU-307] [SUP-605]
### Known Issues
- For CAN FD applications the bootloader must either be configured on non-FD busses or configured for hardware bootloader. CAN FD is only supported in C code. Please contact support prior to using it. [KSB-525]
- CAN FD baudrate calculation function user_can_fd_calculate_baudrate can calculate wrong baudrates some sample point baudrate combinations. [KSB-525] [/intern]
- IOdef baudrates for CAN FD can be wrong for baudrates can be wrong for some baudrate sample point combinations. [KSB-525] [/intern]


## BSW_V3.7.0 (20220704)
### General
### Features
- add CANNEXTGEN support [/intern]
### Fixes
- fix bug which caused MGW 2IO variant to cause a dysfunctional application image [MTNRCG-1118]
- fix bug inside IO_DEF regarding moduleharwarecode_init of the CC16WP 401380 variant [KSB-1025]
- fix CC16WP 401782 variant inside IO_DEF which caused error message when flashing the image [SUP-671]
### Known Issues
- For CAN FD applications the bootloader must either be configured on non-FD busses or configured for hardware bootloader. CAN FD is only supported in C code. Please contact support prior to using it. [KSB-525]
- CAN FD baudrate calculation function user_can_fd_calculate_baudrate can calculate wrong baudrates some sample point baudrate combinations. [KSB-525] [/intern]
- IOdef baudrates for CAN FD can be wrong for baudrates can be wrong for some baudrate sample point combinations. [KSB-525] [/intern]


## BSW_V3.6.0 (202220614)
### General
- S32K142 support [/intern]
- general RAM memory optimization
- RAM memory optimization by makeing some configurations const (can_db_tables, configs) [KSB-973] [/intern]
- added special MRS BSS section for better memory utilization (can_db_ram, can fifos) [KSB-898] [/intern]
### Features
- [beta feature] Add function to user_api to set the CAN baud rate of the bootloader + application temporary, untill the next module restart ( user_can_set_bl_baudrate(baudrate) ). By setting the CAN_BL_BAUDRATE_MODE to "1" the baud rate changes will take permanent effect, the module will boot with the new baud rate on every module start. (CAN_BL_BAUDRATE_MODE define is inside user_code.h). [KSB-994]
- Internal Improvements for future products.
- add S32K142 debug configurations to eclipse project [/intern]
- add MCCR support [/intern]
- add MCCM5A support [/intern]
- CI: improve CI performance by reducing number of jobs. See entries after this for details [/intern]
- CI: build all bootloader variants in a single job. NOTE: bootloader variants shouldn't be build here anyway. They should be built and used from bootloader repo. We could then lose a submodule. [/intern]
- CI and makefile: build all (relevant) libraries in a single job. To get this working changes to the makefile and deploy script are necessary. While big and rather complex these changes will hopefully make the general process more logical. Libraries now can be clearly differentiated between HW_TYP during development. This should make it unnecessary to clean and build libraries everytime developers switch between hardware targets. To keep compatible with Applics and old projects we revert back to the old behavior during deployment for Applics. [/intern]
- Updated variant of 401395 to support a different BOM. [CUS000001-636]
- Better names for some CC16WP variants to improve variant selection process in Applics. [CUS000001-636]
### Fixes
- small syntax fix inside user_code.c examples.
- fixed error in modulehardwarecode_init for both CC16WP LIN variants which caused compile time errors when creating a new project with these variants. [SUP-656]
- fix MCU HW Def and improve with 401912 and 401914 [SUP-661]
- revert the const of the pin_mux table to fix the module reset bug [KSB-1017] [/intern] 
### Known Issues
- For CAN FD applications the bootloader must either be configured on non-FD busses or configured for hardware bootloader. CAN FD is only supported in C code. Please contact support prior to using it. [KSB-525]
- CAN FD baudrate calculation function user_can_fd_calculate_baudrate can calculate wrong baudrates some sample point baudrate combinations. [KSB-525] [/intern]
- IOdef baudrates for CAN FD can be wrong for baudrates can be wrong for some baudrate sample point combinations. [KSB-525] [/intern]


## BSW_V3.5.0 (20220426)
### General
### Features
- add CC16WP variant 401912 (CAN-LIN without INAs) to the IO_DEF. [CUS000001-577]
- add CC16WP variant 401914 (CAN-LIN with 4 INAs) to the IO_DEF. [CUS000001-582]
### Fixes 
- fix typo and improve user_can_fd_send_buffer() function example code in user_code.c [KSB-979]
### Known Issues
- For CAN FD applications the bootloader must either be configured on non-FD busses or configured for hardware bootloader. CAN FD is only supported in C code. Please contact support prior to using it. [KSB-525]
- CAN FD baudrate calculation function user_can_fd_calculate_baudrate can calculate wrong baudrates some sample point baudrate combinations. [KSB-525] [/intern]
- IOdef baudrates for CAN FD can be wrong for baudrates can be wrong for some baudrate sample point combinations. [KSB-525] [/intern]


## BSW_V3.4.0 (20220411)
### General
- official support for CAN FD via user_code. Now it is possible to send CAN FD messages of up to 64 bytes via user_api.
- New user_api function created: "user_can_fd_msg_buffer_send(...)" please refer to the user_code examples for further information.
- Switching the bus to CAN FD mode is possible through the CAN settings in Applics Studio.
### Features
- add CC16WP variant 401782 (CAN-CAN without CAN & LIN WU) to the IO_DEF. [KSB-956]
- add CC16WP variant 401882 (CAN-CAN without INAs) to the IO_DEF. [CUS000001-522]
- Added user_api to send CAN FD messages with up to 64 bytes payload either with BRS enabled/disabled. [KSB-948]
- Made code more modular for CAN FD and non CAN FD mode. Removed precompile CAN FD flags in the hal. The hal is now able to handle data independent of the payload size. Payload size is now declared in sfl, either data[8](for CAN classic) or data[64](for CAN FD) [KSB-948] [/intern]
- The threshold, when a logic high is detected on an analog input used as digital input, is now set to 6V instead of previosly 2/3 of the maximum input voltage. The old logic can still be used or customized by setting the DI_EMULATION_THRESHOLD parameter inside usercode.h [KSB-931]
### Fixes 
- refactored the sfl baudrate init function to have parameter list. Its necassary now to provide a CAN handle with the associated can_db config for the bus. [KSB-948] [/intern]
- fix IO_DEF variant bug inside modulehardwarecode_header for CC16WP 50 mOhm Shunts variant [KSB-967] [/intern]
### Known Issues
- For CAN FD applications the bootloader must either be configured on non-FD busses or configured for hardware bootloader. CAN FD is only supported in C code. Please contact support prior to using it. [KSB-525]
- CAN FD baudrate calculation function user_can_fd_calculate_baudrate can calculate wrong baudrates some sample point baudrate combinations. [KSB-525] [/intern]
- IOdef baudrates for CAN FD can be wrong for baudrates can be wrong for some baudrate sample point combinations. [KSB-525] [/intern]


## BSW_V3.3.0 (20220222)
### General
### Features
- improved the IO_DEF export/generation specification for a more automated IO_DEF generation in the neat future [KSB-566] [/intern]
- CC27 HW_TEST improvements and adaptions [KSB-932] [/intern]
### Fixes 
- fix hal_freq_get_duty() and hal_freq_get_freq() function bug, which occasionally returned a wrong result [SUP-553] [KSB-929] [FAEEU-302]
- fix LIN stack master receive behaviour (primarily related to CC16WP) [KSB-883]
- fix user_eeprom_read_module_serial_nr() function. The serial number is now returend instead of the device number [KSB-935] [SUP-554]
### Known Issues
- For CAN FD applications the bootloader must either be configured on non-FD busses or configured for hardware bootloader. CAN FD is only supported in C code. Please contact support prior to using it. [KSB-525]
- CAN FD baudrate calculation function user_can_fd_calculate_baudrate can calculate wrong baudrates some sample point baudrate combinations. [KSB-525] [/intern]
- IOdef baudrates for CAN FD can be wrong for baudrates can be wrong for some baudrate sample point combinations. [KSB-525] [/intern]


## BSW_V3.2.0 (20220125)
### General
- general BSW internal improvements and cleanup
### Features
- add CC16WP variant 401666 (CAN-CAN WU CAN + KL15) to the IO_DEF. [/intern]
- dummy merge the "more_accurate_PI_controller" branch into the "master" branch [KSB-805] [/intern]
- Power Distributor Module (PDM) is now supported on master branch [KSB-919] [/intern]
- bootloader and bootloader protocol updated to version v24. Look into S32K BL repository for changelog. [KSB-916] [KSB-867] [/intern]
- refactored the sfl baudrate init function to take parameters. Now every bus has to be initialized indivually from the can_app. Needs a valid CAN handle and a can_db config struct with the calculated bit timings for the baudrate. [KSB-888] [/intern]
### Fixes 
- swap the comments in the IO_DEF of the MGW, regarding the assigned connector pin to the resembling HSD(1,2) pin.  [SUP-538]
- fix typo in usercode.c files. Lookup table example used uint16_r instead of uint16_t variable.
- fix the comments in the IO_DEF (MGW) regarding the assigned connector pin to the resembling HSD(1,2) pin. The pins where switched on SW side prior to this fix. [SUP-538] [/intern] 
- fix typo in all usercode.c files. Lookup table example used uint16_r instead of uint16_t variable. [/intern]
- fix SCI interface bugs for power distributor module (PDM) [KSB-920] [/intern]
### Known Issues
- For CAN FD applications the bootloader must either be configured on non-FD busses or configured for hardware bootloader. CAN FD is only supported in C code. Please contact support prior to using it. [KSB-525]
- CAN FD baudrate calculation function user_can_fd_calculate_baudrate can calculate wrong baudrates some sample point baudrate combinations. [KSB-525] [/intern]
- IOdef baudrates for CAN FD can be wrong for baudrates can be wrong for some baudrate sample point combinations. [KSB-525] [/intern]


## BSW_V2.12.0 (20211119)
### General
- CC16WP will now automatically enable 10V output on VREF when LIN is configured and it's required for LIN operation on the hardware. Currently this applies to 1.154.211.00 CC16WP CAN-LIN 12V (400948).[KSB-857]
- Added/improved LIN comments in lin_db_tables and usercode. [BSW-143] [KSB-804] [/intern]
- updated readme. Added some comments on supported devices, corrected project structure, updated hal_s32k_impl readme description on how to use the cpu modules. [KSB-830] [/intern]
- fixed issue with HW bootloader which caused no response from the bootloader after a application was flashed to the module. [KSB-889] [/intern]
- Bootloader and bl_protocol has been updated to v24. New production release (production/release5) was created and delivered into PIM. For more information about the changes look into the bootloader changelog and CHANGELOG_production.md. [KSB-867] [/intern]
### Features
- added new user_api function to clear the CAN TX fifo buffer manually [KSB-756]
- added new user_api function to reset reset_counter. This is necessary, if the application runs for less than 60s at a time and we're on bl up to v21. BL up to v21 also counts normal power-on-reset as a reset. [MRSSUP-236] [KSB-891] [/intern]
- digital readout of ADC pins is now faster. The pins can be configured as GPIO but still be sampled by the ADC. If an ADC pin is not configured for GPIO (e.g. IO_Def has not been updated yet) the previous method will be used, where an analog value is compared against DI_EMULATION_THRESHOLD. The new method may slightly change the behavior of digitized analog inputs since DI_EMULATION_THRESHOLD and the threshold of the GPIO hardware can differ. The CC16WP IO_Def has been updated to use GPIO configuration for analog inputs. Others will be updated in the future, see KSB-881 [KSB-848] [/intern]
### Fixes
- removed hardware dependency from HAL: Number of Hardware Pins is now a parameter for the init function [KSB-847] [/intern]
- makefile prompted some warnings when compiled via command line (-d option for find command is depricated -depth is more posix conform). [KSB-768] [/intern]
- makefile didn't removed bin/ directory. If the clean target was applied the libs for the S32K148 were still present resulting in some unintended behavior of the build process. [KSB-884] [/intern]
- added a missing bracket in hal when compiling with CAN FD flag. [KSB-768] [/intern]
- revert pre-compilation of user_api library, all user_api files are now compiled at Applics Studio compile time. [KSB-896] [SUP-504] [/intern]
- fix bug inside IO_DEF modulehardwarecode.c regarding the 400948 CC16WP variant [KSB-857] [/intern]
- fix the bootloader dependent message box initialization. The initialization process is now handled only by sfl_bl_protocol and bl_protocol. [KSB-908] [KSB-874] [/intern]  
### Known Issues
- For CAN FD applications the bootloader must either be configured on non-FD busses or configured for hardware bootloader. CAN FD is only supported in C code. Please contact support prior to using it. [KSB-525]
- CAN FD baudrate calculation function user_can_fd_calculate_baudrate can calculate wrong baudrates some sample point baudrate combinations. [KSB-525] [/intern]
- IOdef baudrates for CAN FD can be wrong for baudrates can be wrong for some baudrate sample point combinations. [KSB-525] [/intern]


## BSW_V2.11.0 (SKIPPED)
- release was skipped because of lack of development time and resources.

## BSW_V2.10.0 (20211015)
### General
### Features
- added new debug configurations to preserve bootloader and eeprom, this allows to test eeprom depended functions and behave more like an release application [KSB-877] [/intern]
- changed directory structure: moved obj and lib into bin folder [KSB-825] [/intern]
- update user_api_pwm function names. the function names should be the ones the customer uses, the #define should only be used for backwards compatability. As it should be a change the user doesn't notice, this is internal. [/intern]
### Fixes
- fixed wrong msg box ids in CAN_Callback_Error() (reallocation came in RC1 and was noticed in RC5) [KSB-840] [/intern]
- revert `user_pwm_set_pwm()` to behave like versions prior to 2.9.0. While this change makes sense for some users, we found that most use cases are better suited with the old behavior. If you want to keep the behavior from 2.9.0 and know what you're doing please use `user_pwm_set_pwm_immediate()` instead. [KSB-818] [KSB-871] [SUP-479]
- fixed wrong EEPROM address when reading bootloader bus id (did not work up to including RC 3) [APPLICS-1162] [/intern]
- fixed selecting bootloader mailbox address length based on value in EEPROM (did not work up to including RC 3) [APPLICS-1162] [/intern]
- fixed filter on mailbox 1 [APPLICS-1162] [/intern]
- The CAN filter configuration is now largely controlled by Applics or more precisely the defines in can_db_tables.h. If no CAN telegrams for the CAN bus are defined the CAN filter configuration is now "all IDs open" per default. Only special case is when bus is configured for gateway, because gateway requires filters to be open. [KSB-862]
- fixed build breaking default baud rates when creating a new AS project with a CC16WP 1CAN variant. [KSB-855]
- fixed failed communication with LIN Bus as a slave. [KSB-852]
- reset submodule "sdk" since it referenced a WIP branch. [KSB-852] [/intern]
- change the behavior sfl_can_db_output_to_bus. it will now simply skip the transmission of a CAN message if the transmit fails. The retry will occur the next time sfl_can_db_output_to_bus is called and start from the beginning. The only downside could be that the transmission fifo gets saturated at the same point again. But this should be solved by better transmission fifo handling (bigger fifo, less bus load) and not via global counter variable. It will also simplify adding a user_api for this feature. [KSB-806] [/intern]
- is now able to send CAN, CAN FD and CAN FD with BRS dynamically. Its able to decide if a CAN message is FD with BRS or not via user_code. However, if a can message is sent via user_code and the payload is >8 then this message will be sent as CAN FD (without BRS) [KSB-827] [/intern]
- fixed makefile to include user_sources subdirectories to any depth. [KSB-861]
- long_byte_typ used for splitting word (legacy name from HCS08, uint32_t datatype) in uint8_t chunks and back no available in big and little endian. not the nicest implementation but at least it can now be used explicitly. [KSB-870] [/intern]
### Known Issues
- For CAN FD applications the bootloader must either be configured on non-FD busses or configured for hardware bootloader. CAN FD is only supported in C code. Please contact support prior to using it. [KSB-525]
- CAN FD baudrate calculation function user_can_fd_calculate_baudrate can calculate wrong baudrates some sample point baudrate combinations. [KSB-525] [/intern]
- IOdef baudrates for CAN FD can be wrong for baudrates can be wrong for some baudrate sample point combinations. [KSB-525] [/intern]


## BSW_V2.9.1 (20210909)
### Features
- none
### Fixes
- wrong entry in version numbers -> changed to V2.9.1 [/intern] [KSB-842]


## BSW_V2.9.0 (20210906)
### Release notes
- Splitted the main makefile under project root into three additional submakefiles: src.mk(located under src/),  and app_user.mk(located under src/app/user_sources). Now the object and include paths declaration is automated for the app/ subdirectory resulting in a simpler process of adding user defined source files. Please follow the instructions found in src/app/user_sources.
### Features
- io-def version 45: added can-fd support and predefined baudrates for can-fd for all s32k products (some baudrates are not yet tested) [KSB-525] [/intern]
- user_api_basic.c and canCom1.c are now device independent, can pins for initialisation are are now a parameter in modulehardwarecode [KSB-525] [/intern]
- can_db does support can-fd frames with dlc up to 15 (64 byte) [KSB-525]
- user_api user_can_msg_buffer_send supports now can-fd frames up to DLC 15 (64 byte)  [KSB-525]
- experimental user api baudrate calculation and set functions for advanced user added [KSB-525] [/intern]
- local can handles and duplicates removed, switched to central can handle implementation with get fucntion for all can functions in hal, sfl and userapi [KSB-525] [/intern]
- can_db_tables.c/.h support for can-fd added [KSB-525]
- diverse cleanups in can implementation (hal, sfl, user_api) [KSB-525] [/intern]
- hw_testprojects and clean ds projects updated to new user_api version  [KSB-525] [/intern]
- improved automated build on build server by more structuring the pipeline and implementig the "needs" keyword. Libraries are now built upfront and used in all later jobs (except standalone jobs). Deploy jobs can be trigered manually on feature branches. [KSB-653] [/intern]
- fixed bugs from KSB-525, removed user_api_basic and moved its content to can_app [KSB-529] [/intern]
- unified main.c of all S32K HW variants to further improve the variant handling [KSB-808] [/intern]
- splitted the makefile into src.mk and app.mk which contain the definition of the include paths and the objects. The src.mk file contains the vpath, header include paths and object defintion for the src/ subdirectory. Automated the definition of header include paths and object defintion for the src/app/ subdirectory. [KSB-558]
- simplified make targets for the library generation. Changed the names of the object variables and libraries to be conform to the directories. [KSB-558] [/intern]
- modified prepare_to_deploy.sh to not depend on line numbrs anymore. Now uses pattern "MRS_REMOVE_FOR_RELEASE_START" and "MRS_REMOVE_FOR_RELEASE_END" to remove lines between them in the makefile. [KSB-558] [/intern]
- United lpuart bsp configuration, renamed and updated sci_app (former rs232_app) [/intern] [KSB-820]
- added user api functions for changing baudrate and additional parameter for uart. [KSB-820]
- BSW_VARIANT_INFO.mf file is created on the build server which will give informations about which hardware is hidden behind the SYSTEM_VARIANT number [KSB-773] [/intern]
- unified clock configuration [KSB-822] [/intern]
- added new function to alter the FTM synchronization timing, allows writing FTM buffer values instantly into the registry [KSB-818][/intern]
- add the short commit SHA to the version.h file for a better internal traceabilitiy of the software [/intern]
- can filter id from eeprom now used [/intern] [KSB-837]
### Fixes
- Switched the pin configuration for DO_HSD1_OUT and DO_HSD2_OUT in MGWs flextimer.c. They were mixed up: DO_HSD1_OUT was on PWM_HSD2_OUT and vice versa. [MGWS32K-125] [/intern]
- modified Systick_Handler to invoke reimplemented function osif_Tick in baremetal.c instead of referencing a pointer variable. Also removed MRS dependencies in osif_baremetal.c [KSB-780] [/intern]
- CAN filter implementation, now the CAN filter and masks will be considered with RX CAN blocks [KSB-803]
- reconfigured occuppation of the message buffers in connection with CAN filter, found in sfl_can_init(...), CAN_Callback_Error(...) and hal_can_send(...) [KSB-803] [/intern]
- PI control optimized / revised for execution speed. For current measurement at the INAx outputs "user_ai_get_filtered(enum_adc_pin_name pin);" may only be used, with AI_INA_OUT0 - AI_INA_OUT7 as parameter. [SUP-402]
- Comment on user_ai_get_filtered (AI_INA_OUTx) inside user_code() corrected and expanded [/intern]
- added handling for missing filter defines for can app [/intern][KSB-822]
- remove CC16WP CAN-CAN 50mOhm Shunts variant from IO_DEF [/intern]
- Small corrections to the PI controller for the correct behavior of the Kp / Ki factors [/intern]
- fixed bug where *user_set_pwm(...)* had to be called a second time in order to change the PWM frequency and duty cycle. [KSB-818]
- Fixed sfl_bl_protocol incompatibility with new can-fd, filter changes [KSB-837] [/intern]
### Known Issues
- bootloader does not support CAN-FD [KSB-525] 
- can-fd baudrate calculation function user_can_fd_calculate_baudrate can calculate wrong baudrates some sample point baudrate combinations. [KSB-525][/intern]
- io-def baudrates for can-fd can be wrong for baudrates can be wrong for some baudrate sample point combinations. [KSB-525][/intern]


## BSW_V2.8.0 (20210802)
### Features
- update MGW LIN enum numbering, UART0-SCI_TXD_2-X101_5_IO2_LIN2 is now displayed as LIN_BUS_0. UART2-SCI_TXD_1-X101_8_IO1_LIN1 is now displayed as LIN_BUS_1 [MGWS32K-117] [/intern]
- automatic LIN master pullup switching for MGW (depending on the selected node) if supported by the hardware variant [KSB-657]
- re-factored "sfl_can_send_message()" to accept a data buffer instead of 9 consecutive variables (future CAN FD compatibility) [/intern]
- in sfl_can_db utilized more memcpy instead of for-loops in order to copy data buffers [/intern]
- added functionality to be able to readback a PWM digital output pin. [CUS000001-202]
- created user_api function to force a PWM output to either 100% or 0%. Used if a PWM pin is operated as a digital output [KSB-781] [/intern]
- added several new user_api functions for sfl_can_db. Functions are: user_can_db_block_received, user_can_db_test_dp_value, user_can_db_send_dp, user_can_db_get_value_on_change, user_can_db_transmit_deactivate. Added documentation with code examples into user_code [KSB-756]
- initialize LIN signals with default values from LDF. [/intern]
- Cleanup and restructure the LIN test projects inside the S32K repo [KSB-785] [/intern]
- added support for powerdistributor Rev.C, updated hwtest project and IO_DEF.[PWRMOD-166] [\intern]
- add support for FMA and moisture control except the external ADC. [FMAMC-17] [/intern]
### Fixes
- fix code that caused the ADC filtered calculations to always be to low. For calibrated inputs the values would be added to the filter twice. [KSB-782] [/intern]
- Always execute ADC autocalibration except if the valid but outdated identifier "28083" (CC16WP v1) is written to EEPROM. This way we avoid bugs and make sure that all future modules will run AutoCalibration by default. It also fixes a bug where the previously used HW_CALIBRATION_SUPPORT was configured and used wrongly since a couple of versions. (hard to say for each module because it depends on IODef as well. So a lot of combinations/variations to check) (All CC16WP modules produced with BL v21 will have 28084 identifier as they have AutoCalibration enabled at production time. Images: 70000458 - 70000461) [KSB-617] [KSB-763] [KSB-782] [/intern]
- fix recovery for LIN transceivers in modulhardwarecode_sleep() [MGWS32K-113]
- fixed swapped arguments in user_can_api.h, id_type and can_dlc in the "user_can_msg_buffer_send()" declaration [KSB-???] [/intern]
- removed CC16WP CAN-CAN goldplated variant. Customer specific variants should not be shown in AS [KSB-795] [/intern]
- user_api_system.h include was missing inside user_code.c, this caused a compiler warning when using the user_api_reset function [KSB-718]
- fix datatype of membitarray inside graph_support.c file to match the datatype of it's declaretion inside graph_code.c [KSB-797] [APPLICS-1074] [/intern]
- reverted the changes made to hal_pwm_set_duty, to force the PWM output to 100% or 0% via software output control, and outsourced it into a seperate function instead (user_force_pwm_output). [KSB-781] [/intern]
- added more const keywords in user_api_can and sfl_can_db [KSB-???] [/intern]
- dynamic size LIN queue access [KSB-767] [/intern]
- corrected LIN request/response flag handling [KSB-788]
- corrected MGW IO-DEF for variant 401265 (make interfaces for IO1 visible) [MGWS32K-123] [/intern]
- moved the sfl_bl_protocol_s32k_init() call to the beginning of the main function to make sure the EEPROM is initalized early on [KSB-802] [/intern]
- renamed LIN busses for MGW in IO-DEF LIN_BUS_0 => LIN0, LIN_BUS_1 => LIN1 [MGWS32K-124]
### Known Issues
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]
- lin_check_for_received_frame( ..., TRUE) does not enable response transmission when in slave mode. [/intern]


## BSW_V2.7.0 (20210629)
### Features
- add new CC16WP variant (TNR/PNR: 401517) to the IO_DEF [MVV-417]
- add generation of static user_api library to the makefile [KSB-652] [/intern]
- modified linker directive scripts in order to show flash and SRAM memory consumption [KSB-541] [/intern]
### Fixes
- fixed memory boundaries in flash.ld. Region m_text had incorrect addresses set both in S32K144 and 148. [KSB-541] [/intern]
- configured additional TX message buffer [KSB-762] [/intern]
- fixed message loss when busload reaches >93% if configured as gateway [MGWS32K-92] [/intern]
- remove production IO_DEFs from the AS template zip [KSB-772] [/intern]
- modified the behavior of the PWM pins when used as a digital output. Now forces the channel to output either 100% or 0% duty cycle by using the swoctrl register [KSB-733] [/intern]
- improved the behaivor of the PWM pins which are used as digital outputs, by forcing the PWM output to 0% respectively 100% duty cycle [SUP-355]
- add documentation regarding filtered and calibrated values to the user_code examples [APLICS-754] [/intern]
### Known Issues
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]


## BSW_V2.6.0 (20210531)
### Features
- add dither feature to PWM signal, function name is user_pwm_set_dither() inside user_api_pwm.c/.h [KSB-712]
- BL protocol fixed hardware port/pin redundancy field handling [KSB-745] [/internal]
- BL protocol fixed hardware bootloader behaviour [KSB-724]
- BL protocol updated to check and realize application or standalone application operations at runtime, thus having a single BL protocol library for both cases [KSB-694] [/intern]
- implement counter function to count pulses on any frequency input pin, init function to configure the pulse counter (only on HAL level currently) [KSB-731] [SUP-349] [/intern]
- add shell script to add MRS app header to .elf files, add (untested) generic app debug config for pemicro which loads the .s19 file (with app header) instead of the elf (w/o app header) and preserves bootloader as well as eeprom. [KSB-???] [/intern]
- add pwr_distr support to master branch [/intern]
- add IO_DEFs for MGW and CC16WP which support the CAN_IO_Test.exe for hardware testing via Bootloader in production [KSB-661] [/intern]
- add eeprom functions for reading factory data to user_api_eeprom [KSB-614]
### Fixes
- spelling fix inside lin_diagnose.c files of all HW modules [MTNBSW-12] [/intern]
- fix BL protocol for abnormal propagation of internal watchdog into application [KSB-743] [/intern]
- fix BL protocol for Stop User/MRS function and removed reset counter increment upon Stop [KSB-739] [/intern]
- fix IO_DEF bug where second LIN bus was not enabled for MGW TNR/PNR 401271, this caused a bug when trying to enter sleep mode [MGWS32K-114]
### Known Issues
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_V2.5.0 (20210428)
### Features
- MGW: refactored IO_DEF variants, updated the ZNRs and variant descriptions, added new variant (401930) [KSB-708]
- Updates and feature additions in the course of CC16WP CAN-CAN 50 mOhm shunts variant (TNR: 401380) [CUS000001-87] [/intern]
  - ICT now adds 6 new values to EEPROM (temperature, temperature voltage, 5V internal correction value and a correction value for each external reference 5V/8V5/10V) [CUS000001-87] [/intern]
  - HW test project was updated and adapted to match the new variant [CUS000001-87] [/intern]
  - CC16WP CAN-CAN 50 mOhm shunts variant was added to the IO_DEF [CUS000001-87] [/intern]
  - image placed into PIM [CUS000001-87] [/intern]
- activated the ADC auto calibration for CC16WP CAN-CAN 50 mOhm shunts variant, updated modulehardwarecode inside IO_DEF to make sure it is still deactivated for all other CC16WP variants [KSB-617] [/intern]
- add internal temperature voltage measurement to the CC16WP (code, IO_DEF, HW test project) [KSB-233] [/intern]
- add the CC16WP CAN 4INA variant to the HW test project (TNR: 401368) [KSB-692] [/intern]
- Generalization of the calibration function so that it can now also be used by other modules, if supported.[KSB-635] [/intern]
- Adding two new tabs in the IO Def - ANALOG_PIN_DEF - which contains information for calibration.[KSB-635] [/intern] 
### Fixes
- fix visible INAs for TNR: 401230 module inside CC16WP IO_DEF to match available INAs on the hardware [KSB-612]
- fix LIN communication bug caused by re-initialization of SCI interface through SCI_init() call after lin_init() call [KSB-713]
- fix error in IO_DEF regarding 401368 CC16WP wrong io_tables.c/.h generation [APPLICS-950] 
- fix ADC clock from div by 8 to div by 1 which will let the ADC conversion run at 40 MHz [KSB-???] [/intern]
- fix PWM_MAX bug inside hal_impl which treated PWM_MAX enum as valid PWM pin [KSB-???] [/intern]
- fix bug where giving a PWM enum parameter into the user_do_set() function would cause undefined behaviour [APPLICS-966]
- fix some wrong/out-dated examples and define names inside user_code.c [KSB-705]
- fix merge error regarding zeroing of the SysTick_CVR register in the osif_UpdateTickConfig() function [KSB-733] [/intern]
- fix visibility of KL30 measurement enable pin in IO_DEF [MGWS32K-108] [/intern]
- fix switched CAN baudrates inside MGW IO_DEF [MGWS32K-111] [/intern]
### Known Issues
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_V2.3.0 (20210305)
### Features
- reduced complexity of ftm module. FTM module can no be configured completely out of the io-def, just add every pin configuration the hw support to the full config in bsp_cfg. Also switching ADC and freqeuncy input while device is running is now possible. [KSB-561] [/intern]
- add new CC16WP variant to IO_DEF: 401368 (for Pöttinger) [MTNRCG-293] [/intern]
- add CAN error watermark function user_can_get_error() (returns error + timestamp) [KSB-434]
- refactoring ADC: add multiplexing support for ADC channels. [KSB-609] [KSB-681] [KSB-682]
### Fixes
- fix the SNS signals measurement, they are now multiplexed (fixes "known issue" KSB-316) [KSB-316]
- cleanup repository and submodules branches [KSB-674] [/intern]
- FTM frequency capture clock settings use system clock now [KSB-561] [/intern]
- fix names of the multiplexed SNS inpust to AI_HSDx_CURx [KSB-675] [/intern]
- fix IO_DEF bug: old function call of shift_reg() was still present in some CC16WP variants [APPLICS-922] 
- fix name check in the SNS calibration handling [KSB-675] [/intern]
### Known Issues
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v2.2.0 (20210209)
### Features
- refactor and improve user_api_eeprom [KSB-610] [KSB-614]
  - new user_eeprom_read_raw function to read the whole EEPROM - even data reserved for MRS use. There's also a user_eeprom_write_raw, but write operations to reserved areas are still blocked. If you use these functions use EE_USER_START to get the start adress for user data. [KSB-632]
  - NOTE: data structure and contents in reserved EEPROM areas are subject to change. Always use APIs to access data such as application version and serial number.  - new functions to directly read/write 32bit values: user_eeprom_read_value_32bit, user_eeprom_write_value_32bit()
  - new function to read serialnumber: user_eeprom_read_module_snr()
- add user_ai_get_filtered() to get filtered calibrated adc values. todo: make sure this works for all ADC channels not just for calibrated. [/intern]
- merge user_apis of all hardware platforms. they now all use the same code. NOTE: Some compiler macros still remain. [/intern]
- user_api_uart now has multi-instance handling. you will now need to provide. This is done in preparation for future hardwares. [KSB-595]
- extend user_do_readback(). It will now also read back virtual pins (shift_reg) [/intern]
- move code for hardware test project to src/ds_HW_TEST since it only contains ds code and eventually we might be able to merge ds and ds_HW_TEST directories. for now it at least makes comparions e.g. using beyond compare much easier. [/intern]
- moved user_ai_set_measurement_range() replaced with modulhardwarecode_adc_set_measurement_range(). [/intern]
- cleanup/update .ds file templates: remove unnecessary data regenerated by applics/iodef [/intern]
- defaults Application Version and Name changed to "V0.0.0" and "Modulename Application". This should make it more obvious that these fields can and should be edited by the user.
- add hal_i2c_master_set_slave_address() function to HAL [/intern]
- hal_sci changes allow to set NULL pointer as callbacks (used to disable callbacks) [/intern]
- deploy script is unified for all hardwares, no more double zipping of the template on the build server [KSB-644] [/intern]
### Fixes
- include sfl_math.h in user_api_util.h so users don't have to in user_code.c [KSB-620]
- move functions out of user_api_basic and if possible directly into modulhardwarecode. This will speed-up development and ease adding new modules. [KSB-610] [/intern] [/integration]
  - shift_reg_app removed, causes changes to io_tables.c
- update modulhardwarecode.c/.h inside IO_DEF [KSB-632] [/intern] 
- fix spelling error of hal_pwm_...._gobal to global [KSB-619] [/intern]
- increased m_text area (flash) to max. available amount for S32K144 MCUs [KSB-626]
- updated interrupt disable/enable functions to use hal_sys and not the INTERRUPT handling marco and adapt sfl_fifo to it [/intern]
- moved BL code from sfl files to bl_protocol files [KSB-471] [/intern]
- LIN cleanup inside SFL, and HAL [KSB-648] [/intern]
- fix LIN bug where pre-compiled libraries would not work with user created lin_db_tables. [KSB-665]
- hotfix inside lin_db_tables.h for two LIN busses which are both configured as master [KSB-671] [/intern]
- fix communication interfaces related bugs (LIN, CAN) [KSB-668] [SUP-278]
- fix Applics project update bug inside BSW [APPLICS-889]
- fix bug inside user_ai_get_cal(...) function [KSB-684]
- scan response time is fixed to resolve problems when scanning more then on module [KSB-655] [SUP-285]
### Known Issues
- On CC16WP only four current sense signals for `OUT[0-7]` (`AI_SNS[0-7]`) can be accessed at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.14.0 (20201130)
### Features
### Fixes
- fix compile errors which occurred in BSW_V1.13.0 regarding user_can_send_msg_buffer, user_can_db_transmit_deactivate, LUT_MODE_EXTRAPOLATION. [KSB-619]
- remove Rev. B and Rev. B1 of the CC16WP 2MB CAN CAN (401081) from IO_DEF, they were only prototype modules and will therefore not be supported. They will no longer apear in the select module dialog of the Applics Studio. [KSB-588]
### Known Issues

## BSW_v1.13.100 (20201126)
### Features
### Fixes
- Fixed logical LIN-Bus to physical SCI assignment. This is now done completely via IO_DEF [/KSB-603][/intern].
- Generate compiler warning if Watchdog is disabled [/intern].
- The graph cycle time could deviate/jitter if usercode() had a long execution time. This is fixed with this release. [KSB-616]
- Update the Bode HW test src (CAN tables, over current recognition, changed user api functions to new naming scheme) and adapt the PCAN panel. [BODE-142] [/intern]
- fix the Bode modulhardwarecode.c template [BODE-142] [/intern]
- update the Bode clock settings inside clockMan1.c [BODE-142] [/intern]
### Known Issues

## BSW_v1.13.0 (20201104)
### Features
- add user_api_eeprom to write Application Version and Application Name. Add example usage to usercode_init(). This data is displayed when you perform a scan using MRS Flasher. [APPLICS-326]
- increase stack size for 2MB variants to 4KB - same as for other variants.
- refactor adc handling: move several functions from calibr to adc_app and rename them accordingly. Applics Project Update should auto-migrate but you can also manually search and replace it in your code. Please note: you also need to use the header (adc_app.h) for the new functions. [KSB-591]
  - calibr_update() --> adc_processing() [/intern]
  - calibr_get_value() --> user_ai_get_cal()
  - calibr_signal_adc_ready() --> adc_signal_ready() [/intern]
  - calibr_get_module_type() --> adc_get_module_type() [/intern]
- seperate the adc table config structure into adc table config structure and adc table result structure [KSB-591] [/intern]
- use the new generated adc result structure to save the adc results in several different forms (calibration, filter, raw, weighted) if available. [KSB-591] [/intern]
- add support for new in-development hardware wbox [WBX-270] [/intern]
- lots of internal clean-up in hw test projects, CI, deploy scripts and Cyclone configs [/intern]
- fix MGW project.png [/intern]
- integrate latest Bootloader Protocol v15-beta2 [/intern]
- prepare removal of unneeded safety mechanisms (no simultaneous PD and PU for CC16WP) in shift_reg_app.c [/intern]
### Fixes
- Fixed problem with the number of channels initilisated of PWM and freq inputs, caused by a precompiled max value. [/KSB-575] [/intern]
### Known Issues
- On CC16WP only four current sense signals for `OUT[0-7]` (`AI_SNS[0-7]`) can be accessed at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.12.3 (20201021)
### Features
### Fixes
- fix/improve variant naming for CC16WP and MGW [KSB-606]
### Known Issues
- Only four current sense signals for `OUT[0-7]` (`AI_SNS[0-7]`) can be accessed at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.12.2 (20201015)
### Features
### Fixes
- Add missing CC16WP 2MB CAN CAN ISOBUS variant to IO_DEF [MVFAE-167]
- Update BL and bl_protocol with small CAN error recovery improvments and default baudrate fix to 125 KBit/s [KSB-585] [/intern]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.12.1 (20201013)
### Features
### Fixes
- Fix bug inside IO_DEF for revision check before flashing with the MRS flasher [KSB-574]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.12.200 (20201012)
### Features
- Support of CC16WP Rev. C modules inside the IO_DEF [KSB-574]
- bl_protocol of the application supports BL_v13 [KSB-581]
### Fixes
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.12.0 (20201006)
### Features
### Fixes
- Fix bug where a S32K148 image could be flashed onto a S32K144 module and vice versa [KSB-569]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.11.101 (20200930)
### Features
### Fixes
- Fix compile warnings in Applics Studio [KSB-538]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.11.100 (20200929)
### Features
- added first version of current control feature to BSW. This is curently only documented inside user_code.c file [KSB-261]
- MGW template is now supporting Applics Studio [KSB-???] [/intern]
### Fixes
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.11.50 (20200924)
### Features
### Fixes
- adapt the CC16WP Rev. C IO_DEF to match the Rev. B version [/intern]
- Refactroring signing.bat to remove IUG specific informations [KSB-551] [/intern]
- Reduce and cleanup HW testprojects variants for CC16WP and Bode [KSB-231] [/intern]
- Improve makefile regarding SYSTEM_TARGET and SYSTEM_BUILD variables [KSB-531] [/intern]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.11.0 (20200901)
### Features
### Fixes
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.10.101 (20200827)
### Features
- added memory usage print [KSB-478]
### Fixes
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.10.100 (20200825)
### Features
- analog inputs can now be used as digital inputs [KSB-484]
### Fixes
- Adapt the clock settings to use PLL or SOSC as source for peripheral clocks [KSB-460] [/intern]
- Verify zeroing of S32_SysTick->CVR has no inpact on BSW [KSB-209] [/intern]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.10.50 (20200811)
### Features
- Analyse the build system rearding GCC versions on build server and on local machines to make sure build result is the same on both systems [KSB-326] [/intern]
- The GCC version is displayed in the build log window of Applics Studio [KSB-494]
- Toolchain update: GCC (arm-none-eabi-gcc) was updated to version 8.3.1 [KSB-490]
### Fixes
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.10.0 (20200804)
### Features
### Fixes
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.9.101 (20200730)
### Features
- Updated USER API and HAL functions for pwm and frequency measurement. [KSB-508]
- Renaming of hal_pwm and user_api_pwm functions to minimize confusion. [KSB-301] [KSB-508] [/intern]
- hal pwm and supports now both s32k144 and s32k148.[KSB-364] [/intern]
- Frequency capturing does support all FTM modules, not just FTM 3. [KSB-322] [/intern]
- Implemented HAL watchdog hardware independance. [KSB-408] [/intern] 
### Fixes
- gcc fix for gitlab ci [/intern]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.9.100 (20200728)
### Features
- Added variant configuration for bsw, hw-test and updated ci [KSB-481] [/intern] 
### Fixes
- Fix lin db template  [KSB-481] [/intern]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.9.3 (20200717)
### Features
### Fixes
- Fix compile error caused by missing variable (deleted during LIN refactoring) [KSB-481] [/intern]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.9.2 (20200716)
### Features
### Fixes
- User API EEPROM functions fixed, updated examples inside user_code.c [KSB-498]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.9.1 (20200714)
### Features
- Add first beta support for CC16WP CAN-LIN and CC16WP CAN-LIN 12V variants [KSB-181]
### Fixes
- User API EEPROM functions fixed, updated examples [KSB-498]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.9.0 (20200707)
### Features
### Fixes
- Fix compile warning which occured after CAN filter adaptions [/intern]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.8.102 (20200706)
### Features
### Fixes
- Refactor the build call to fix/reduce complexity of the dsl_cfg.h [APPLICS-582] [APPLICS-585] [/intern]
- Updated comments in user_code.c to be up-to-date with current implementation state [KSB-474] [/intern]
- modulehardwarecode_cyclic() function is called every main cycle now, instead of everly graph_cycle_time. [KSB-475] [/intern]
- Setting CAN filters in Applics Studio is now supported on BSW side [KSB-432]  
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.8.101 (20200702)
### Features
- Added CAN Bus error recovery [APPLICS-427]
- Added CAN Bus error detection [APPLICS-426]
### Fixes
- EEPROM: restrict access of the user_api_eeprom functions to user eeprom space, update AS Doc accordingly [KSB-472] [/intern]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.8.100 (20200630)
### Features
- implemented disable/enable interrupt when entering sleep [KSB-335] [/intern]
- Fifo handling is now configurable by Applics Studio in can_db_tables [KSB-402] [APPLICS-561] [/intern]
- Fifo size adjustable through CAN Settings in Applics Studio. [APPLICS-561]
- implemented CAN gateway feature (configureable through Applics Studio). [KSB-427] [KSB-436]
### Fixes
- Implemented user api changes defined by FAE [KSB-400] [/intern]
  - user_ai_get() and user_ai_get_mv() now directly return the ai value. No need for passing pointers.
- IO-def update: use MCU_TYP name defined inside CPU_PINDESCR.mdb [KSB-398] [/intern]
- Moved can fifo from hal to sfl [KSB-402] [/intern]
- removed hw specific parts from hal [KSB-403] [/intern]
- removed hw specific parts from sfl [KSB-409] [/intern]
- removed compiler warning in sfl_math and sfl_can_db (maybe-uninitialized and unused-but-set-variable warnings). [KSB-286]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.8.0 (20200529)
### Features
### Fixes
- updated README.md with supported hardware modules and more details [/intern]
- internal code and project structure cleanup [KSB-399]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- `CAN Gateway` feature (configured through Applics Studio) doesn't work. Users are advised to implement CAN gateway functionality in `user_code`.
- can_fifo_config shouldn't be in user_api_basic and needs to be moved to code parts generated by applics studio in the future [/intern]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]
- Compiler warning in sfl_math and sfl_can_db (maybe-uninitialized and unused-but-set-variable warnings). [KSB-286]

## BSW_v1.7.3 (20200518)
### Features
### Fixes
- fix: user defined baudrates for both can bus now now working. [APPLICS-510] [KSB-401]
- Ext. Baud defines were removed. Field prop was added to can_bus_db_const. Data for ext baud comes now from can_bus_db_const. [APPLICS-510] [KSB-401] [/intern]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- `CAN Gateway` feature (configured through Applics Studio) doesn't work. Users are advised to implement CAN gateway functionality in `user_code`.
- can_fifo_config shouldn't be in user_api_basic and needs to be moved to code parts generated by applics studio in the future [/intern]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.7.2 (20200515)
### Features
### Fixes
- fix merge bug where second and wrong irq_cfg was added to bsp_cfg [/intern]
- quick-fix: user defined baudrates are now working [APPLICS-510] 
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- `CAN Gateway` feature (configured through Applics Studio) doesn't work. Users are advised to implement CAN gateway functionality in `user_code`.
- can_fifo_config shouldn't be in user_api_basic and needs to be moved to code parts generated by applics studio in the future [/intern]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.7.1 (20200512)
### Features
### Fixes
- fix bug where user_do_set(...) did not work with PWM (DO_HSDx_PWMx) pins [KSB-406]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- `CAN Gateway` feature (configured through Applics Studio) doesn't work. Users are advised to implement CAN gateway functionality in `user_code`.
- can_fifo_config shouldn't be in user_api_basic and needs to be moved to code parts generated by applics studio in the future [/intern]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.7.0 (20200506)
### Features
- added first beta support for CC16WP Rev. C. Template generation added to scripts and git ci. [KSB-379] [/intern]
### Fixes
- cleanup the s32k_support branch of sfl submodule (adpat to master branch strucutre) [KSB-178] [/intern]
- fix bug which caused compile error when project path contained spaces [APPLICS-481]
- first version with static code analysis using axivion bauhaus suite [KSB-103] [/intern]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- `CAN Gateway` feature (configured through Applics Studio) doesn't work. Users are advised to implement CAN gateway functionality in `user_code`.
- can_fifo_config shouldn't be in user_api_basic and needs to be moved to code parts generated by applics studio in the future [/intern]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.6.0 (20200407)
### Features
- Both MCU derivates (S32K144 and S32K148) are supported on master branch, new Applics Studio template contains S32K144 and S32K148 libs [KSB-287] [/intern]
- Add beta support for CC16WP 2MB CAN-CAN module. Note: This product is not yet finished [KSB-287]
- Add NULL pointer check in EEPROM function [KSB-166] [/intern]
### Fixes
- Changed default baudrate to 125 kBit/s in .ds file [KSB-346] [/intern]
- Review of the user_api with FAE, rework on user_code coments [KSB-263] [/intern]
- Add user_api_system_power files to user_api with function for SystemSoftwareReset [KSB-232] [/intern]
- Fix user example for user_eeprom_read in user_code [KSB-166] [/intern]
- Fix inconsistent CAN baudrates inside .ds file [APPLICS-471] [/intern]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- `CAN Gateway` feature (configured through Applics Studio) doesn't work. Users are advised to implement CAN gateway functionality in `user_code`.
- can_fifo_config shouldn't be in user_api_basic and needs to be moved to code parts generated by applics studio in the future [/intern]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]


## BSW_v1.5.0 (20200303)
### Features
- all IO pins (including shift register pins) can now be accessed through the same api (`user_do_set()`). This removes the earlier workaround. [KSB-312]
- improved user EEPROM functionality. Added offset for the EEPROM customer area. Out-of-bounds protection for the EEPROM user area implemented. [KSB-330]
- remove support for CC16WP Rev. A, add support for Rev. B1 [KSB-303]
- sending and receiving messages over CAN is now using SW fifos (size can be defined in `user_code.h`). This reduces the burden on the user application to check if CAN bus is occupied.[KSB-297]
- multiple improvements to generated code structure to prepare for future HW variants and to help improve graphical programming [/intern]
- improved the IO_DEF according to the description in KSB-303 [KSB-303] [/intern]
- Add dsl_cfg.h to include list inside graph_support.h to make the MCU_TYPE and HW_TYPE defines accessible for GP Lib [KSB-303] [/intern]
- add option to test LIN functionality in test_src/user_code.c and PCAN-Explorer (send CAN MSG: LIN0_DP_U_BATT + 100 offset) [KSB-229] [/intern]
- add option to test SCI functionality in `user_int_rx_sci()` (send response, received char + 0x01 offset) [KSB-229] [/intern]
- new experimental `user_api_SCI` for SCI/UART communication [/intern]
### Fixes
- Fix modulhardwarecode_sleep bug, where CAN would not recover, if sleep conditions weren't met during going to sleep [KSB-306] [KSB-334]
- Fix the measurement range select function user_ai_set_measurement_range() (former adj_ai_meas_range), switching measurement range will now select the right adc_vref value [KSB-304]
- fixed problem with CAN tx transmission errors [KSB-297]
- fixed problem with CAN tx transmission errors, which was caused by setting CAN to freeze mode, to get extended id flag. Extended CAN flag comes now from code an status field. [KSB-297] [/intern]
- fix CAN Tx sequencing issue by removing usage of multiple hw message buffers for CAN Tx. [KSB-297]
- fix CAN db single id gateway functionality. (column `CAN Bus GW` in `CAN DB Editor`) [KSB-297]
- Fix typo in IO_DEF "CAN_BAUDTRATE_DEF" table [KSB-295] [/intern]
- improved the IO_DEF according to the description in KSB-303 [KSB-303] [/intern]
- removed faulty implementation of user_api_ai pull config, VREF config and shift register handling [KSB-312] [/intern]
- update hal_def (#f448779) and hal_impl (#9b6a01f) submodules and fix some typedef names inside user_api (pwm errorcodes typedef) to adjust to newest hal_def [KSB-281] [/intern]
- Temporary fix for measurement range set until Multi IO supoort is added. Using user_ai_set_measurement_range() function once for a AI pin will ignore the adc_vref value given by the IO_DEF and only use the adc_vref values from the struct in user_api_ai.c file. Also moved user_ai functions from user_api_io to user_api_ai [KSB-304] [/intern]
- CHANGELOG.md format was adjusted to Applics Studio changelog format [KSB-280] [/intern]
- graphcode: Fix mem16array datatype from signed int to uint16_t due to the fact that signed int on a 32 bit MCU is int32_t [KSB-328] [/intern]
- hal: update to newer version (changes to i2c, spi, which aren't relevant for current products) [/intern]
- fix sfl_can_db.c which caused compile error on non 2 CAN hardware variants. [KSB-336] [APPLICS-420] [/intern]
- fixed can rx/tx fifo and rewrote its functions to be can number independent. [KSB-336] [/intern]
- fix BSW for IO_DEF v24 changes, adapt IO_DEF [/intern]
### Known Issues
- Because there is no automatic multiplexing, users can only access four current sense for `OUT[0-7]` (`AI_SNS[0-7]`) at a time. Use `AI_INA_OUT[0-7]` as an alternative. [KSB-316]
- `CAN Gateway` feature (configured through Applics Studio) doesn't work. Users are advised to implement CAN gateway functionality in `user_code`.
- can_fifo_config shouldn't be in user_api_basic and needs to be moved to code parts generated by applics studio in the future [/intern]
- modulhardwarecode_init should not require user_api code (similar to sleep). This needs to be removed in the future. [/intern]

## BSW_v1.4.4 (20200129)
### Features
- none
### Fixes
- fix compile error which occured because "CAN_BUS_1" enum is not given in non CAN-CAN variants [KSB-279] [/intern]

## BSW_v1.4.3 (20200128)
### Features
- none
### Fixes
- update variant information inside IO_DEF [KSB-279] [/intern]
- fix the CAN bus numbers inside CC16WP_XXX.ds which led to wrong can_db_tables initialization [KSB-283] [/intern]

## BSW_v1.4.2 (20200127)
### Features
- none
### Fixes
- fix more IO_DEF inconsistencies for variant management (ZNR_TNR_DEF table) [KSB-279] [/intern]
- small bug fixes which occured because of moduhardwarecode adaptions [KSB-279] [/intern]
- fix the comments of the MC_AI_SNS inputs inside the IO_DEF to resemble the right PWM output channels [APPLICS-248]

## BSW_v1.4.1 (20200124)
### Features
- none
### Fixes
- fix IO_DEF inconsistencies for variant management [KSB-279]

## BSW_v1.4.0 (20200121)
### Features
- implemented cleanup of the makefile via "app_utils/scripts/prepare_for_deploy.sh" script before release template creation. [KSB-154] [/intern]
- update the version naming scheme from ASW_vX.X_CC16WP_vX.X.X to a simpler BSW_vX.X.X due to the fact that expirience showed that the "Anwendungssoftware (ASW)" and "Basesoftware (BSW)" were versionized together in one step in most cases. Version number changes from 1.3.0 to 0.7.0
- provide the interrupt priority configuration file to the user (location: src/app/irq_cfg.c). [KSB-276]
- CC16WP variant information (c-code) is now generated from the IO_DEF, BSW libraries are variant independent. [KSB-258] [/intern]
- support for all CC16WP variants added [KSB-258]
### Fixes
- fix/refactor the git ci procedure to get a more clearer result on the build server with better defined names for build stages and build targets. [/intern]
- fix PWM bug, where PWM outputs connected to the same FTM module were falsely initilized with 100% duty cycle. [KSB-262]
- remove the configuration of the ADC irq priority inside the ADC_init function, irq prioritization is only defined in irq_cfg.c file. [KSB-276]
- fix bug where scanning the module with the MCU Flasher and new Applics Studio CAN Flasher was not possible under some circumstances. [KSB-244]

## ASW_v0.5_CC16WP_v1.3.0 (20191219)
### Features
- none
### Fixes
- fix bug where it was not possible to use the PWM pins as digital outputs with graphcode block "DIGOUT_S32K" and user_code function "user_do_set()"
- fix missing 1ms timer (should have been added with ASW_v0.5_CC16WP_v1.2.3)

## ASW_v0.5_CC16WP_v1.2.9 (20191217)
### Features
- none
### Fixes
- fix the adc_vrefh value in the IO_DEF which is used for converting the raw digit value of the adc channel into the millivolt/milliampere value
- integration fix which led to the Applics Studio not including the user_sources folder into the release template.

## ASW_v0.5_CC16WP_v1.2.8 (20191216)
### Features
- The io_tables source and header file is now generated from the IO_DEF.mdb by the Applics Studio (The IO_DEF was updated)
### Fixes
- create user_sources directory and change path for user_sources to src/app/user_sources

## ASW_v0.5_CC16WP_v1.2.7 (20191212)
### Features
- add base software version header (can be used to identify the version of the base software, do not confuse with application software version "src/app/version.h")
- increased stack size from 1042 KB to 4096 KB
### Fixes
- none 

## ASW_v0.5_CC16WP_v1.2.6 (20191205)
### Features
- add creation of all images to auto build process
### Fixes
- fix version in dsl_cfg.h
- enable CAN receive regardless of existing message in can_db_tables

## ASW_v0.5_CC16WP_v1.2.5 (20191204)
### Features
### Fixes
- important Applics Studio integration fix, baudrate changeable via application 

## ASW_v0.5_CC16WP_v1.2.4 (20191202)
### Features
- add the possibility for the customer to add their own source files to the build process (in directory: src/user_sources)
- add flag in user_can_msg_send() and user_can_msg_buffer_send() function to specify the CAN ID format (extended or standard)
- fix AI_ID conversion factor. AI_ID measurement is now as accurate as expected. [KSB-235], [APPLICS-127], [APPLICS-116]
### Fixes
- fix the missing includes for the user_api files
- update sfl module to fix user_can_msg_receive() function

## ASW_v0.5_CC16WP_v1.2.3 (20191127)
### Features
- activate FPU
- add four more EEPROM fields for SNS signals calibration data
- always add PCAN Panel and CHANGELOG to all releases/build artifacts
- User API improvments: added user_api_SCI
- Usercode improvments: added user_int_timer_1ms() in user_code()
- added sw_version and module_name to the eeprom in usercode_init, from dsl_cfg.h
- created test file for module variant with 2x CAN, in test_src/ds
- modified makefile to easily create a test file, DS_DIR_ERSATZ
- sw version management is now based on bootloader version management
- change bootloader versioning to simple, incrementing number
- improve CI toolchain to auto-build CC16WP bl+app (for 2xCAN variant)
### Fixes
- The correct pin number is set to read the current sense state of the second high side driver
- fix properties settings for CS_HSD1 and CS_HSD2 in PCAN panel


## ASW_v0.5_CC16WP_v1.2.2 (20191108)
### Features
- small improvement to calibration functions
- small LIN stack improvments
- User API improvments
### Fixes
- fix bug where LIN was blocking main loop

## ASW_v0.5_CC16WP_v1.2.1 (20191107)
### Features
### Fixes
- remove debug info from release image
- small general improvments

## ASW_v0.5_CC16WP_v1.2.0 (20191106)
### Features
- Added INA/SNS calibration functionality
- Added first version of the new USER API
### Fixes
- Re-organizing the project/directory structure

## ASWv0.4.0_CC16WP_v1.1.2.4 (20190806)
### Features
- CC16WP features are supported up to HAL level or graphcode level
- PWM now adjustable between 0 - 1000 per mil (MRS standard conform)
- Frequency and duty cycle measurement implemented and tested
- Adjusted clock settings for 16 MHz resonator
- Add support for building diffrent images for diffrent CC16 variants
### Fixes
- adjusted project template for better Applics Studio support

## ASWv0.3.1_CC16WP_v1.0.0.0 (20190425)
### Features
- editing and compiling with MRS DSL (can_db_tables and graph_code)
- get ADC values in mV
- flashing of application via bootloader implemented
### Fixes
- fix "standard" CAN number for CC16WP
- fix missing includes in graph_support.h
- fix compile errors (which occured during MRS DSL integration)

## ASW_v0.0 (20190315)
### Features
Initial CHANGELOG
- Standard MRS CAN DB handling
- Set PWM output
- Set, reset and read digital I/Os
- Communication via SCI (UART)
### Fixes
- none
