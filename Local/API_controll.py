import paramiko
import math
from datetime import datetime
import zipfile
import os

class SSH_local:
    def __init__(self,host, username, password,path,file_name):
        self.file_exe = file_name
        self.path = path
        self.path_run = path+"/"+file_name+" "
        self.ssh = paramiko.SSHClient()
        self.host=host
        self.username = username
        self.password = password
    def connect(self):
        self.ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        self.ssh.connect(self.host, username=self.username, password=self.password)
        
    def compile(self,file_cpp_name,library):
        command = 'cd '+self.path+';'+'g++ -o '+self.file_exe+" "+file_cpp_name+" "+library
        self.ssh.exec_command(command)
        # print(stdout.read().decode())
    def read_angle(self):
        stdin, stdout, stderr = self.ssh.exec_command(self.path_run+'0 0 0 0')
        out = stdout.read().decode()
        i = -2
        while(out[i]!=' '):
            i-=1
        try:
            offset = int(out[i+1:-1])
        except:
            print("Error: ",out)
        # print("Offset angle: "+str(offset))
        return offset
    
    def go_to_offset_angle(self,offset):
        loop=1
        while(loop):
            angle = self.read_angle()
            print("Angle now: "+str(angle/1024*360))
            dir = 0
            if angle > offset:
                if angle - offset < 512:
                    loop = round((angle-offset)/32)
                else:
                    loop = round((1024-angle+offset)/32)
                    dir = 1
            else:
                if offset - angle < 512:
                    loop = round((offset-angle)/32)
                    dir = 1
                else:
                    loop = round((1024-offset+angle)/32)
            if loop:            
                command = '5 '+str(dir)+' 0 ' + str(loop)
                print("Waiting ...")
                stdin, stdout, stderr = self.ssh.exec_command(self.path_run+command)
                print(stdout.read().decode())
        print("Offset position!")
        # turn off led 3 - go to offset position
        self.control_led(3,0)
        # self.ssh.exec_command(self.path_run+'0 0 0 -2')
        
    def choose_step(self):
        step = int(input("Step: 1-180 | 2-90 | 3-45 | 4-22.5 | 5-11.25: "))
        while step not in {1,2,3,4,5}:
            print("Step is not correct, try again!")
            step = int(input("Step: 1-180 | 2-90 | 3-45 | 4-22.5 | 5-11.25: "))
        return step
    
    def choose_direction(self):
        dir = int(input("Direction: 0-follow clockwise | 1-counter clockwise: "))
        while dir not in {0,1}:
            print("Direction is not correct, try again!")
            dir = int(input("Direction: 0-follow clockwise | 1-counter clockwise: "))
        return dir
    
    def choose_angle(self):
        angle = float(input("Total angle: "))
        while angle <0 and angle>360:
            print("Angle is not correct, try again!")
            angle = float(input("Total angle: "))
        return angle
    
    def get_loop_from_angle(self,angle,step):
        return round(angle/(180/math.pow(2, step-1)))
    
    def control_led(self,led_number,status):
        self.ssh.exec_command(self.path_run+'0 0 0 '+str(-led_number*2+status))
        
    def capture_image_full(self,localpath):
        step = self.choose_step()
        dir = self.choose_direction()
        angle = self.choose_angle()
        loop = self.get_loop_from_angle(angle,step)
        if abs(angle - 360) < 0.01:
            loop -= 1
        
        file_name = datetime.now().strftime("%d%m%Y-%H%M")        
        print("Folder save Images: ",file_name)
        
        # turn on led 3 - not in the offset position
        self.control_led(3,1)
        
        print("loop: "+str(loop+1))
        command = str(step)+' '+str(dir)+' 1 '+str(loop)+ ' '+file_name
        print("Capturing {} images...".format(loop*2+2))
        stdin, stdout, stderr = self.ssh.exec_command(self.path_run+command)
        data = stdout.read() + stderr.read()
        print(data.decode())
        print("Captured {} images!\n".format(loop*2+2))
        
        print("Zipping images ...")
        stdin, stdout, stderr = self.ssh.exec_command("cd /media/DCIM;zip -r "+file_name+".zip "+file_name)
        print(stdout.read().decode())
        print("File's zipped\n")
        
        print("Copying zip file to the local ...")
        ftp_client=self.ssh.open_sftp()
        ftp_client.get('/media/DCIM/'+file_name+'.zip',localpath+file_name+'.zip')
        print("File {} has been copied to the computer in the folder {}".format(file_name+'.zip',localpath))
        
        # ssh.exec_command(path+'0 0 0 -3')
        # remove folder and file in stereoPi
        self.ssh.exec_command("cd /media/DCIM;rm "+file_name+".zip ")
        self.ssh.exec_command("cd /media/DCIM;rm -r "+file_name)
        
        print("Unzipping ...")
        with zipfile.ZipFile(localpath+file_name+".zip","r") as zip_ref:
            zip_ref.extractall(localpath)
        print("Done!")
    
    def capture_single_image(self,localpath):
        file_name = datetime.now().strftime("%d%m%Y-%H%M")        
        print("Folder save Images: ",file_name)
        
        # turn on led 3 - not in the offset position
        self.control_led(3,1)
        command = '0 0 1 0 '+file_name
        print("Capturing 2 images...")
        stdin, stdout, stderr = self.ssh.exec_command(self.path_run+command)
        data = stdout.read() + stderr.read()
        print(data.decode())
        print("Captured 2 images!\n")
        
        print("Zipping images ...")
        stdin, stdout, stderr = self.ssh.exec_command("cd /media/DCIM;zip -r "+file_name+".zip "+file_name)
        print(stdout.read().decode())
        print("File's zipped\n")
        
        print("Copying zip file to the local ...")
        ftp_client=self.ssh.open_sftp()
        ftp_client.get('/media/DCIM/'+file_name+'.zip',localpath+file_name+'.zip')
        print("File {} has been copied to the computer in the folder {}".format(file_name+'.zip',localpath))
        
        # ssh.exec_command(path+'0 0 0 -3')
        # remove folder and file in stereoPi
        self.ssh.exec_command("cd /media/DCIM;rm "+file_name+".zip ")
        self.ssh.exec_command("cd /media/DCIM;rm -r "+file_name)
        
        print("Unzipping ...")
        with zipfile.ZipFile(localpath+file_name+".zip","r") as zip_ref:
            zip_ref.extractall(localpath)
        os.remove(localpath+file_name+".zip")
        print("Done!")
    def rotate_platform(self):
        # step = self.choose_step()        
        angle = self.choose_angle()
        loop = self.get_loop_from_angle(angle,5)
        dir = self.choose_direction()
        command = '5 '+str(dir)+' 0 ' + str(loop)
        print("Waiting "+str(loop)+" rotating steps...")
        self.ssh.exec_command(self.path_run+command)
        print("Done rotating!")
        
        