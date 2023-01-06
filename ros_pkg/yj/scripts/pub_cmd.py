#!/usr/bin/env python3

import rospy
from std_msgs.msg import String

from config import Config as cf
from utils.audio_utils import AUDIO_UTILS

class Cmd:
    def __init__(self):

        if cf.listen:
            self.au = AUDIO_UTILS()

        self.pub = rospy.Publisher("/cmd", String, queue_size=10)

    def pub_cmd(self):

        while not rospy.is_shutdown():
            if cf.listen:
                frames = self.au.listen()

                if len(frames) > 0:
                    cmd = self.au.wave_to_str(frames)
                    print(cmd)
                    if len(cmd) > 0:
                        self.pub.publish(cmd)
            
            #text
            else:
                print("YOU:", end="")
                cmd = input()
                self.pub.publish(cmd)
                rospy.sleep(1)

if __name__=='__main__':
    rospy.init_node('pub_cmd', anonymous=False)
    cmd = Cmd()

    try:
        cmd.pub_cmd()
        #rospy.spin()
    except rospy.ROSInterruptException:
        pass
