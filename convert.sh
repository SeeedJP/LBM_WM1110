#!/usr/bin/bash

echo
date

################################################################################
# LBM

cd src/lbm

echo
echo "=== LBM"
git describe --always --tags --dirty
git show | head -1

#rm -rf .git
rm Makefile
rm -r makefiles
rm smtc_modem_core/lr1mac/src/lr1mac_class_b/smtc_d2d.c
rm -r smtc_modem_core/radio_drivers/sx126x_driver
rm -r smtc_modem_core/radio_drivers/sx128x_driver
rm -r smtc_modem_core/smtc_modem_crypto/soft_secure_element
rm smtc_modem_core/smtc_ral/src/ral_llcc68*
rm smtc_modem_core/smtc_ral/src/ral_sx126x*
rm smtc_modem_core/smtc_ral/src/ral_sx128x*
rm smtc_modem_core/smtc_ralf/src/ralf_sx126x*
rm smtc_modem_core/smtc_ralf/src/ralf_sx128x*
rm -r utilities

lbm_headers=`find -type f -name "*.h"`
lbm_sources=`find -type f -name "*.c"`

echo
for f in ${lbm_headers} ${lbm_sources}
do
    echo Convert ${f}
    for h in ${lbm_headers}
    do
        if [ `dirname ${h}` != `dirname ${f}` ]
        then
            h=${h/.\//}
            sed -i "s/#include \"`basename ${h}`\"/#include \"lbm\/${h//\//\\\/}\"/" ${f}
        fi
    done
done

cd ../..

################################################################################
# MW

cd src/mw

echo
echo "=== MW"
git describe --always --tags --dirty
git show | head -1

#rm -rf .git
rm -r apps
rm -r host_driver
rm -r lora_basics_modem
rm -r shields
rm -r smtc_hal
rm -r toolchain
rm -r geolocation_middleware/doc

mw_headers=`find -type f -name "*.h"`
mw_sources=`find -type f -name "*.c"`

echo
for f in ${mw_headers} ${mw_sources}
do
    echo Convert ${f}
    for h in ${mw_headers}
    do
        if [ `dirname ${h}` != `dirname ${f}` ]
        then
            h=${h/.\//}
            sed -i "s/#include \"`basename ${h}`\"/#include \"mw\/${h//\//\\\/}\"/" ${f}
        fi
    done
    for h in ${lbm_headers}
    do
        if [ `dirname ${h}` != `dirname ${f}` ]
        then
            h=${h/.\//}
            sed -i "s/#include \"`basename ${h}`\"/#include \"lbm\/${h//\//\\\/}\"/" ${f}
        fi
    done
done
