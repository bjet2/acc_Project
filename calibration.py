'''
A program to reduce the work during calibration process. Needs to changed after 
reading datasheet for accelerometer. THis is a generic calibration procedure and will do
for now.
'''
#calibration data
x_max = 263
x_min = -282
y_max = 292
y_min = -256
z_max =287
z_min = -215
#find the offset. 
offset_x = (x_max+x_min)/2
offset_y = (y_max+y_min)/2
offset_z = (z_max+z_min)/2
#find scale
scale_x=2*255/(x_max-x_min)
scale_y=2*255/(y_max-y_min)
scale_z=2*255/(z_max-z_min)
#remind formula
print(" rough formula is new_x = (old_x - offset_x)*scale_x research later")
print(" scale_x = ", scale_x, " offset_x", offset_x)
print(" scale_y = ", scale_y, " offset_y", offset_y)
print(" scale_z = ", scale_z, " offset_z", offset_z)
