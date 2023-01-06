#!/usr/bin/env python3

import rospy
import cv2
import numpy as np

from sensor_msgs.msg import Image

class depth:
    def __init__(self):

        self.color_img = None

        self.color_sub = rospy.Subscriber("kinect2/hd/image_color", Image, self.set_color)
        self.depth_sub = rospy.Subscriber("kinect2/hd/image_depth_rect", Image, self.set_depth)

    def set_color(self, data):
        col_img = np.fromstring(data.data, dtype=np.uint8)
        col_img = col_img.reshape(data.height, data.width, 3)

        self.color_img = col_img

    def set_depth(self, data):

        dep_img = np.fromstring(data.data, dtype=np.uint16)
        dep_img = dep_img.reshape(data.height, data.width)

        dis = str(dep_img[540, 960])

        cv2.circle(self.color_img, (960, 540), 100, (255, 0, 0))
        cv2.putText(self.color_img, dis, (910, 490), cv2.FONT_HERSHEY_SIMPLEX, 10, (255, 0, 0), 20)

        if self.color_img is not None:
            cv2.imshow("output", self.color_img)
            cv2.waitKey(1)

if __name__=='__main__':
    rospy.init_node('get_depth', anonymous=False)
    depth()

    try:
        rospy.spin()
    except KeyboardInterrupt:
        rospy.loginfo("shutting down")
