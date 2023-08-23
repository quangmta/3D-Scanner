from API_controll import SSH_local

host = '192.168.1.75'
username = 'root'
password = 'root'
# path - path of file program in the 3D scanner (in StereoPi)
path = '/media/Program'
# localpath - path of saving result in personal computer
localpath = 'E:/Python/ZoeDepth/input/'
# run_file_name - name of exe file after compiling cpp files (Don't need to change)
run_file_name = 'main'

SSH = SSH_local(host,username,password,path,run_file_name)

SSH.connect()

SSH.compile("stereoPi_new.cpp source/lidarlite_v3.cpp source/GPIO.cpp source/UART.cpp","-I .")

offset = SSH.read_angle()
print("Offset angle: "+str(offset*360/1024))

while True:
    print("")
    print("---- Controll command of 3D scanner ----")
    print("1 - Goto offset position")
    print("2 - Capture images in current position")
    print("3 - Capture images of the surrounding scenes")
    print("4 - Manual rotate")
    print("0 - Exit")
    operation = input("Input corresponding number: ")
    if operation == '0':
        break       
    elif operation == '1':
        SSH.go_to_offset_angle(offset)
    elif operation == '2':
        SSH.capture_single_image(localpath)
    elif operation == '3':
        SSH.capture_image_full(localpath)
    elif operation == '4':
        SSH.rotate_platform()
    # elif operation == '10':
    #     SSH.control_led(1,0)
    # elif operation == '11':
    #     SSH.control_led(1,1)
    # elif operation == '20':
    #     SSH.control_led(2,0)
    # elif operation == '21':
    #     SSH.control_led(2,1)
    # elif operation == '30':
    #     SSH.control_led(3,0)
    # elif operation == '31':
    #     SSH.control_led(3,1)

SSH.ssh.close()    
