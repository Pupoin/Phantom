#%%
import argparse
import os
import tempfile
import numpy as np
import SimpleITK as sitk


def read_raw(binary_file_name, image_size, sitk_pixel_type, image_spacing=None,
             image_origin=None, big_endian=False):
    """
    Read a raw binary scalar image.

    Parameters
    ----------
    binary_file_name (str): Raw, binary image file content.
    image_size (tuple like): Size of image (e.g. [2048,2048])
    sitk_pixel_type (SimpleITK pixel type: Pixel type of data (e.g.
        sitk.sitkUInt16).
    image_spacing (tuple like): Optional image spacing, if none given assumed
        to be [1]*dim.
    image_origin (tuple like): Optional image origin, if none given assumed to
        be [0]*dim.
    big_endian (bool): Optional byte order indicator, if True big endian, else
        little endian.

    Returns
    -------
    SimpleITK image or None if fails.
    """

    pixel_dict = {sitk.sitkUInt8: 'MET_UCHAR',
                  sitk.sitkInt8: 'MET_CHAR',
                  sitk.sitkUInt16: 'MET_USHORT',
                  sitk.sitkInt16: 'MET_SHORT',
                  sitk.sitkUInt32: 'MET_UINT',
                  sitk.sitkInt32: 'MET_INT',
                  sitk.sitkUInt64: 'MET_ULONG_LONG',
                  sitk.sitkInt64: 'MET_LONG_LONG',
                  sitk.sitkFloat32: 'MET_FLOAT',
                  sitk.sitkFloat64: 'MET_DOUBLE'}
    direction_cosine = ['1 0 0 1', '1 0 0 0 1 0 0 0 1',
                        '1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1']
    dim = len(image_size)
    header = ['ObjectType = Image\n'.encode(),
              (f'NDims = {dim}\n').encode(),
              ('DimSize = ' + ' '.join([str(v) for v in image_size]) + '\n').encode(),
              ('ElementSpacing = ' + (' '.join([str(v) for v in image_spacing])
                                      if image_spacing else ' '.join(['1'] * dim)) + '\n').encode(),
              ('Offset = ' + (' '.join([str(v) for v in image_origin]) if image_origin
                  else ' '.join(['0'] * dim) + '\n')).encode(),
              ('TransformMatrix = ' + direction_cosine[dim - 2] + '\n').encode(),
              ('ElementType = ' + pixel_dict[sitk_pixel_type] + '\n').encode(),
              'BinaryData = True\n'.encode(),
              ('BinaryDataByteOrderMSB = ' + str(big_endian) + '\n').encode(),

              # ElementDataFile must be the last entry in the header
              ('ElementDataFile = ' + os.path.abspath(binary_file_name) + '\n').encode()]
    fp = tempfile.NamedTemporaryFile(suffix='.mhd', delete=False)

    # print(header)

    # Not using the tempfile with a context manager and auto-delete
    # because on windows we can't open the file a second time for ReadImage.
    fp.writelines(header)
    fp.close()
    img = sitk.ReadImage(fp.name)
    print(fp.name)
    os.remove(fp.name)
    return img


parser = argparse.ArgumentParser()
parser.add_argument('raw_file_name', help='path to raw binary image file')
parser.add_argument('out_file_name',help='output file name when image read as little endian')
parser.add_argument('size', nargs=3, help="image size, x,y,...", type=int)
parser.add_argument('spacing', nargs=3, help="image spacing, x,y,...", type=float)
parser.add_argument('position', nargs=3, help="image position, x,y,...", type=float)
parser.add_argument("big_endian", default=2, type=lambda v: v.lower() in {"1", "true"},
                    help="\'false\' for little ending or \'true\'for big "
                    "endian")
parser.add_argument('--sitk_pixel_type', default='sitkUInt8', help="SimpleITK pixel type (e.g. sitk.sitkUInt16)")
parser.add_argument('-z','--zoom', dest="zoom", help="zoom ...", type=float, default=1.)
args = parser.parse_args()
print(args)

spacing = [args.spacing[0]*args.zoom, args.spacing[1]*args.zoom, args.spacing[2]*args.zoom]
position = [args.position[0]*args.zoom, args.position[1]*args.zoom, args.position[2]*args.zoom]
print(spacing)
print(position)
string_to_pixelType = {"sitkUInt8": sitk.sitkUInt8,
                       "sitkInt8": sitk.sitkInt8,
                       "sitkUInt16": sitk.sitkUInt16,
                       "sitkInt16": sitk.sitkInt16,
                       "sitkUInt32": sitk.sitkUInt32,
                       "sitkInt32": sitk.sitkInt32,
                       "sitkUInt64": sitk.sitkUInt64,
                       "sitkInt64": sitk.sitkInt64,
                       "sitkFloat32": sitk.sitkFloat32,
                       "sitkFloat64": sitk.sitkFloat64}

# Read the image using both big and little endian
image = read_raw(binary_file_name=args.raw_file_name,
                 image_size=args.size,
                 sitk_pixel_type=string_to_pixelType[args.sitk_pixel_type],
                 image_spacing=spacing,
                 image_origin=position,
                 big_endian=args.big_endian)
sitk.WriteImage(image, args.out_file_name)

# image = read_raw("lobster_301x324x56_uint8.raw",
#                  [301, 324, 56],
#                  sitk_pixel_type=string_to_pixelType["sitkUInt8"],
#                  image_spacing=[0.5,0.5,0.7],
#                  big_endian=False )
# sitk.WriteImage(image, "lobster_301x324x56_uint8.dcm")


# for slice in range(56):
#     image2 = image[:300,:,slice]
#     sitk.WriteImage(image2, "old/lobster_{0}.dcm".format("%05d" % slice))


#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%#%%
# if "SITK_NOSHOW" not in os.environ:
#     sitk.Show(image, 'raw converted')


# resample = sitk.ResampleImageFilter()
# resample.SetInterpolator = sitk.sitkLinear
# resample.SetOutputDirection = image.GetDirection()
# resample.SetOutputOrigin = image.GetOrigin()
# new_spacing = [1, 1, 1]
# resample.SetOutputSpacing(new_spacing)

# orig_size = np.array(image.GetSize(), dtype=np.int)
# orig_spacing = image.GetSpacing()
# new_size = orig_size*(orig_spacing/new_spacing)
# new_size = np.ceil(new_size).astype(np.int) #  Image dimensions are in integers
# new_size = [int(s) for s in new_size]
# resample.SetSize(new_size)

# newimage = resample.Execute(image)
# sitk.WriteImage(image, "hahah.dcm")

#%%
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

