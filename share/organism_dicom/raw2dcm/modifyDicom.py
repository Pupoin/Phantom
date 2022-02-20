#%%
from __future__ import print_function
import argparse
import SimpleITK as sitk
import sys, time, os


parser = argparse.ArgumentParser()
parser.add_argument('input_file_name',help='input file name ...')
parser.add_argument('outputDir', help='outputDir...')
args = parser.parse_args()
print(args)


# Read the original series. First obtain the series file names using the
# image series reader.
# data_directory = sys.argv[1]
# data_directory = "old/"
# series_IDs = sitk.ImageSeriesReader.GetGDCMSeriesIDs(data_directory)
# if not series_IDs:
#     print("ERROR: given directory \""+data_directory+"\" does not contain a DICOM series.")
#     sys.exit(1)
# series_file_names = sitk.ImageSeriesReader.GetGDCMSeriesFileNames(data_directory, series_IDs[0])
# if the Study Instance UID is not the same, using this
# series_file_names=('lobster_301x324x56_uint8.dcm',)
series_file_names=(args.input_file_name,)
series_reader = sitk.ImageSeriesReader()
series_reader.SetFileNames(series_file_names)

# Configure the reader to load all of the DICOM tags (public+private):
# By default tags are not loaded (saves time).
# By default if tags are loaded, the private tags are not loaded.
# We explicitly configure the reader to load tags, including the
# private ones.
series_reader.MetaDataDictionaryArrayUpdateOn()
series_reader.LoadPrivateTagsOn()
image3D = series_reader.Execute()

# Modify the image (blurring)
# filtered_image = sitk.DiscreteGaussian(image3D)
filtered_image = image3D


# Write the 3D image as a series
# IMPORTANT: There are many DICOM tags that need to be updated when you modify an
#            original image. This is a delicate opration and requires knowlege of
#            the DICOM standard. This example only modifies some. For a more complete
#            list of tags that need to be modified see:
#                           http://gdcm.sourceforge.net/wiki/index.php/Writing_DICOM

writer = sitk.ImageFileWriter()
# Use the study/series/frame of reference information given in the meta-data
# dictionary and not the automatically generated information from the file IO
writer.KeepOriginalImageUIDOn()

# Copy relevant tags from the original meta-data dictionary (private tags are also
# accessible).
tags_to_copy = ["0010|0010", # Patient Name
                "0010|0020", # Patient ID
                "0010|0030", # Patient Birth Date
                "0020|000D", # Study Instance UID, for machine consumption
                "0020|0010", # Study ID, for human consumption
                "0008|0020", # Study Date
                "0008|0030", # Study Time
                "0008|0050", # Accession Number
                "0008|0060"  # Modality
]

modification_time = time.strftime("%H%M%S")
modification_date = time.strftime("%Y%m%d")

# Copy some of the tags and add the relevant tags indicating the change.
# For the series instance UID (0020|000e), each of the components is a number, cannot start
# with zero, and separated by a '.' We create a unique series ID using the date and time.
# tags of interest:
# direction = filtered_image.GetDirection()
direction = [1,0,0,0,1,0,0,0,1.4]
# print(direction)
spacing=filtered_image.GetSpacing()
origin=filtered_image.GetOrigin()
size=filtered_image.GetSize()
# print(size)
#%%
series_tag_values = [(k, series_reader.GetMetaData(0,k)) for k in tags_to_copy if series_reader.HasMetaDataKey(0,k)] + \
                 [("0008|0031",modification_time), # Series Time
                  ("0008|0021",modification_date), # Series Date
                  ("0008|0008","DERIVED\\SECONDARY"), # Image Type
                  # ("0020|0032", "-150.5\\-162.0\\0"), # Image position is the absolute values of x, y, and z in the real-world coordinate system
                  ("0020|0032", "{0}\\{1}\\{2}".format(origin[0], origin[1], origin[2])), # Image position is the absolute values of x, y, and z in the real-world coordinate system
                  ("0018|0050", str(spacing[2])),   # Slice Thickness
                  ("0028|0030", str(spacing[0]) + '\\' + str(spacing[1]) ), # Pixel Spacing
                  ("0020|1041", "0" ), # SliceLocation;
                  ("0020|000d", "1.2.826.0.1.3680043.2.1125.1.68893319353041308453793351162003428" ), # Study Instance UID, make slices from the same 
                #   ("0020|0037", "1\\0\\0\\0\\1\\0"),  # Image Orientation (Patient)
                  ("0008|0060", "CT"), 
                  ("0020|000e", "1.2.826.0.1.3680043.2.1125."+modification_date+".1"+modification_time), # Series Instance UID
                  ("0020|0037", '\\'.join(map(str, (direction[0], direction[3], direction[6],# Image Orientation (Patient)
                                                    direction[1],direction[4],direction[7])))),
                #   ("0008|103e", series_reader.GetMetaData(0,"0008|103e") + " Processed-SimpleITK")
                  ] # Series Description
#%%
for i in range(filtered_image.GetDepth()):
    image_slice = filtered_image[:size[0]-size[0]%2,:size[1]-size[1]%2,i]
    # Tags shared by the series.
    for tag, value in series_tag_values:
        image_slice.SetMetaData(tag, value)
    # Slice specific tags.
    image_slice.SetMetaData("0008|0012", time.strftime("%Y%m%d")) # Instance Creation Date
    image_slice.SetMetaData("0010|0010", "lobster") # PatientsName
    image_slice.SetMetaData("0010|0020", "2034204") # PatientsName
    image_slice.SetMetaData("0008|0013", time.strftime("%H%M%S")) # Instance Creation Time
    # image_slice.SetMetaData("0020|0032", '\\'.join(map(str,filtered_image.TransformIndexToPhysicalPoint((0,0,i))))) # Image Position (Patient)
    image_slice.SetMetaData("0020|0013", str(i)) # Instance Number
    image_slice.SetMetaData("0020|1041", str(0+spacing[2]*i )) # Instance Number
    image_slice.SetMetaData("0020|0032", "{0}\\{1}\\{2}".format(origin[0], origin[1], origin[2]+spacing[2]*i)) # Instance Number

    # Write to the output directory and add the extension dcm, to force writing in DICOM format.
    # writer.SetFileName(os.path.join(sys.argv[2],str(i)+'.dcm'))
    # writer.SetFileName(os.path.join(sys.argv[2],"lobster_{0}.dcm".format("%05d" % i)))
    writer.SetFileName(os.path.join("{0}/".format(args.outputDir),"lobster_{0}.dcm".format("%05d" % i)))

    writer.Execute(image_slice)
sys.exit( 0 )
# %%
