#/bin/bash
python convert.py \
    lobster_301x324x56_uint8.raw `# input file`\
    lobster_301x324x56_uint8.dcm `# output file`\
    301 324 56                   `# size `\
    1. 1. 1.4                    `# spacing`\
    -150.5 -162.0 -39.2          `# origin position`\
    False                        `# bigendian`\
    --sitk_pixel_type sitkUInt8  `# sitk_pixel_type`\
    --zoom 0.6                    `# zoom`  \


python modifyDicom.py lobster_301x324x56_uint8.dcm new


# NRRD0004
# Complete NRRD file format specification at:
# http://teem.sourceforge.net/nrrd/format.html
# type: uint8
# dimension: 3
# space: left-posterior-superior
# sizes: 301 324 56
# space directions: (1,0,0) (0,1,0) (0,0,1.4)
# kinds: domain domain domain
# endian: little
# encoding: raw
# space origin: (-150.5,-162.0,-39.199999999999996)
# data file: lobster_301x324x56_uint8.raw
