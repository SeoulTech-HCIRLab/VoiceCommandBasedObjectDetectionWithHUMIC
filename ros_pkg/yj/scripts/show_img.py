#!/usr/bin/env python3

import rospy
import cv2
import numpy as np
import math
import sys
import serial
import time

from std_msgs.msg import String
from sensor_msgs.msg import Image
from darknet_ros_msgs.msg import BoundingBoxes

from yj.msg import Target
from config import Config as cf
from utils.com import COM as c
from utils.target_utils import *

class show:
    def __init__(self):

        self.boxes = list()
        self.cmd = None
        self.targets = list()
        self.target = None
        self.f_done = False
        self.f_pub =False
        
        self.cur_pos = 0
        self.cnt = 0

        self.start = time.time()

        try:
            try:
                self.ser = serial.Serial('/dev/ttyACM1', 115200, timeout=0.1)
            except:
                serial.Serial('/dev/ttyACM1', 115200).close()
                self.ser = serial.Serial('/dev/ttyACM1', 115200, timeout=0.1)
        except:
            print('Serial error')
            sys.exit()

        self.coord_pub = rospy.Publisher("/target/xyxy_depth", Target, queue_size=10)
        self.obj_sub = rospy.Subscriber("/darknet_ros/bounding_boxes", BoundingBoxes, self.set_obj)
        self.cmd_sub = rospy.Subscriber("/cmd", String, self.set_cmd)

        self.image_sub = rospy.Subscriber("/kinect2/hd/image_color", Image, self.set_img)

    def set_obj(self, data):
        self.boxes = data.bounding_boxes

    def set_cmd(self, data):
        self.cmd = data.data.split(' ');

        targets = [box for box in self.boxes
                  if compare_list([box.Class], self.cmd)
                  and box.probability > cf.box_prob_th]

        if len(targets) > 0 or 'start' in self.cmd:
            self.targets = targets
            self.f_done = False
            self.f_pub = False
            self.pub_target(start=True)
            if 'start' in self.cmd:
                self.ser.write('0'.encode('utf-8'))
                self.cur_pos = 0


        if self.f_done:
            if compare_list(self.cmd, c.YES_):
                self.target = self.targets[0]
                self.pub_target(box=self.target, target=True)
                self.f_pub = True
                self.start = time.time()
                
            else:
                print(c.no2)
                self.targets = list()
                self.f_doen = False

        if compare_list(self.cmd, c.DIRECTION_):
            if len(self.targets) > 1:
                self.targets = get_inds(self.cmd, self.targets, c.DIRECTION_)
            elif len(self.targets)==1:
                print(c.no2)
                self.targets = self.starts

        if len(self.targets) > 1:
            print(c.no1)
        
        if len(self.targets) == 1 and not self.f_done:
            print(c.no3)
            self.f_done = True

        #self.pub_target()

    def pub_target(self, box=None, target=False, start=False):
        target_coord = Target()
        target_coord.target = target
        target_coord.start_flag = start

        if box is not None:
            xc = int(box.xmin + (box.xmax - box.xmin) / 2)
            yc = int(box.ymin + (box.ymax - box.ymin) / 2)

            tan_rad = math.atan((xc-960)/1066)
            deg = abs(int((tan_rad*180)/math.pi))

            direc = 0
            if xc-960 < 0:
                direc = 1
            elif xc-960 > 0:
                direc = -1

            target_coord.degree = deg
            target_coord.direction = direc
            target_coord.depth = 0
            target_coord.xc = xc
            target_coord.yc = yc

        self.coord_pub.publish(target_coord)
        
    def set_img(self, data):

        img = np.fromstring(data.data, dtype=np.uint8)
        img = img.reshape(data.height, data.width, -1)

        if self.f_pub:

            self.pub_target(box=self.target, target=True)

            st_ed = time.time() - self.start

            targets = [box for box in self.boxes 
                      if box.Class == self.target.Class]
            xcs = np.array([box.xmin+(box.xmax-box.xmin)/2 for box in targets])
            target = targets[abs(xcs - 960).argmin()]
            boxes = [target]

            if st_ed > 3:
                x1 = target.xmin
                x2 = target.xmax
                xc = x1+(x2-x1)/2
                y1 = target.ymin
                y2 = target.ymax
                yc = y1+(y2-y1)/2
                print(f'xc: {xc} - yc: {yc}')

                #1080/2+-1080/12
                if yc < 450 or yc > 630:
                    rad = math.atan((yc-540)/1060)
                    deg = (rad*180/math.pi)

                    if deg > 5 or deg < -5:
                        pos = int(deg*151875/180)
                        self.cur_pos += pos
                        self.ser.write(str(self.cur_pos).encode('utf-8'))

                self.start = time.time()
        else:
            if len(self.targets) > 0:
                boxes = self.targets
            else:
                boxes = self.boxes

        for box in boxes:
            if box.probability > cf.box_prob_th:
                x1 = box.xmin
                y1 = box.ymin
                x2 = box.xmax
                y2 = box.ymax

                label = box.Class

                color = (0,0,255)

                cv2.rectangle(img, (x1, y1), (x2, y2), color, 3)
                cv2.putText(img, label, (x1, y1),
                            cv2.FONT_HERSHEY_SIMPLEX, 1, color, 2)

        #cv2.rectangle(img, (800, 450), (1120, 630), (0, 255, 0), 3)

        #cv2.namedWindow("output", cv2.WINDOW_NORMAL)
        #cv2.resizeWindow("output", 1728, 972)
        cv2.imshow("output", img)
        cv2.waitKey(3)

if __name__=='__main__':
    rospy.init_node('show_img', anonymous=False)
    show()

    try:
        rospy.spin()
    except KeyboardInterrupt:
        rospy.loginfo("shutting down")

    cv2.destroyAllWindows()

