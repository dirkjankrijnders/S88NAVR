Making production gerbers
=========================

Exporting gerbers from kicad
----------------------------
To create gerbers for a PCB, open the .brd or .kicad\_pcb file in pcbnew. Choose "plot" from the "file" menu. Set the output directory to "Outputs/Rev_x" where x is the current revision number. Also create a drill file is that same directory, with the following settings:

- Units: Inch
- Suppress leading zeros
- Drill Map File Format: Gerber
- No options checked
- Drill origin: Absolute

You should now have at least 6 gerbers and a drill file in the "Outputs/Rev_x" directory.

Merging gerbers into a panel
----------------------------

Install gerbmerge (<http://ruggedcircuits.com/gerbmerge/>). Check the settings in "template_main", especially the revision number in the path. Then generate the gerbmerge config file use the script:

`./make-gerbmerge-config.sh S88-S88N S88P3 >> gerbmerge-config`

Where the last arguments are the basename of the gerbers made in the previous step. Make adjustments to the panel-layout as needed and create the merged gerbers:

`python2.7 ~/Projects/3rd_party/gerbmerge-1.8/gerbmerge/gerbmerge.py gerbmerge-config panel-layout`

The drill file created by gerbmerge are very minimal and not every program likes them, so fix the .xln with the supplied script:

`./gerbmergexln2drl.sh S88-Panel-Rev0`

where the last argument is the basename of the panel gerbers.

Checking the results
--------------------
check the results with gerbv:

`gerbv <basename>.*`

double check with gerbview:

`gerbview <basename>.*`

triple with an online gerber viewer:

- <http://mayhewlabs.com/3dpcb>

If the output of all programs looks good, you're in good shape for production! 