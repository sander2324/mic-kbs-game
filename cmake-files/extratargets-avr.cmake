#
# these extra build targets will always be executed
#

# avrdude path
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(AVRDUDE /usr/bin/avrdude)
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(AVRDUDE /mingw64/bin/avrdude)
endif()

# Strip binary for upload
add_custom_target(strip ALL ${CMAKE_STRIP} ${PROJECT_NAME}.elf DEPENDS ${PROJECT_NAME})

# Transform binary into hex file, we ignore the eeprom segments in the step
add_custom_target(hex ALL ${CMAKE_OBJCOPY} -R .eeprom -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex DEPENDS strip)

# Transform binary into hex file, this is the eeprom part (empty if you don't
# use eeprom static variables)
add_custom_target(eeprom ${CMAKE_OBJCOPY} -j .eeprom  --set-section-flags=.eeprom="alloc,load"  --change-section-lma.eeprom=0 -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.eep DEPENDS strip)

# Make assembly
add_custom_target(disasm ${CMAKE_OBJDUMP} -h --source --demangle --wide ${PROJECT_NAME}.elf > ${PROJECT_NAME}.lss DEPENDS ${PROJECT_NAME})

# Upload the firmware with avrdude
add_custom_target(upload ${AVRDUDE} -P ${AVR_PORT} -c ${PROG_TYPE} -p ${MCU} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)

# Upload the eeprom with avrdude
add_custom_target(upload_eeprom ${AVRDUDE} -P ${AVR_PORT} -c ${PROG_TYPE} -p ${MCU}  -U eeprom:w:${PROJECT_NAME}.eep DEPENDS eeprom)

# Burn fuses
add_custom_target(fuses ${AVRDUDE} -P ${AVR_PORT} -c ${PROG_TYPE} -p ${MCU}  -U lfuse:w:${L_FUSE}:m -U hfuse:w:${H_FUSE}:m -U efuse:w:${E_FUSE}:m -U lock:w:${LOCK_BIT}:m )

# Clean extra files
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.hex;${PROJECT_NAME}.eeprom;${PRODUCT_NAME}.lst;${PROJECT_NAME}.lss")
