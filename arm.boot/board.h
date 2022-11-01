/*
 * Since we are running on QEMU, it is worth looking at QEMU documentation
 * for the emulation of the Versatile boards:
 *
 *   https://qemu.readthedocs.io/en/latest/system/arm/versatile.html
 *
 * Also remember that you can ask QEMU, from the QEMU console, a dump of
 * the information about the memory map and devices:
 *
 *   (qemu) info mtree
 *
 *   (qemu) info qtree
 *
 *  The last one about the "qtree" will give you all the Base Address Register (BAR)
 *  for the different device controllers attached on the bus/interconnect.
 *
 * Also, when necessary, look at the document describing the real "Versatile Application Board"
 * (also called VersatileAB):
 *
 *    Versatile Application Baseboard for ARM926EJ-S (HBI-0118)
 *
 * Also referenced as DUI0225, downloadable from:
 *
 *  http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0225d/I999714.html
 *
 * Look at the memory map, section 4.1, page 137, to find the base addresses for all
 * the attached device controllers.
 */

#ifndef BOARD_H_
#define BOARD_H_

#ifdef BOARD_VERSATILEAB
#include "board_vab.h"
#endif

#ifdef BOARD_VERSATILEPB
#include "board_vpb.h"
#endif

#endif /* BOARD_H_ */
