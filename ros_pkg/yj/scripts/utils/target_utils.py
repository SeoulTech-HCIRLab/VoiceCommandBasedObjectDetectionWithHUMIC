#!/usr/bin/env python3

import numpy as np
import cv2

def compare_list(lis1, lis2):
    if len(list(set(lis1)-(set(lis1)-set(lis2))))>0:
        return True
    else:
        lis1 = ' '.join(lis1)
        if 'middle' in lis1:
            for l2 in lis2:
                if l2 in lis1:
                    return True

        return False

def prepare_ing(mins):
    sort_ind = np.argsort(mins)
    mins = mins[sort_ind]
    mid = int(len(mins)/2)
    median = mins[mid]

    return sort_ind, mins, mid, median

def set_inds(cmd, mins, targets, DIRECTION_):
    sort_ind, mins, mid, median = prepare_ing(mins)

    if compare_list(cmd, [DIRECTION_[0]]):
        inds = np.where(mins > median)
        if len(targets)%2 == 0:
            inds = np.append(np.where(mins == median), inds)

    elif compare_list(cmd, [DIRECTION_[1]]):
        inds = np.where(mins < median)[0][::-1]

    elif compare_list(cmd, [DIRECTION_[2]]):
        inds = np.where(mins == median)
        if len(targets)%2 == 0:
            inds = np.append(np.where(mins == mins[mid-1]), inds)

    return sort_ind, inds

def get_inds(cmd, targets, DIRECTION_):

    flag1 = False

    if compare_list(cmd, DIRECTION_[0:3]):
        xmin = np.array([box.xmin for box in targets])
        sort_ind, inds = set_inds(cmd, xmin, targets, DIRECTION_[0:3])

        inds = sort_ind[inds]
        flag1 = True

    if compare_list(cmd, DIRECTION_[3:6]):
        if flag1:
            targets = [t for i, t in enumerate(targets) if i in inds]

        ymin = np.array([box.ymin for box in targets])
        sort_ind, inds = set_inds(cmd, ymin, targets, DIRECTION_[3:6])

        inds = sort_ind[inds]

    if compare_list(cmd, ['end', 'far']):
        inds = [inds[-1]]
    
    targets = [t for i, t in enumerate(targets) if i in inds]

    return targets

def draw_box(img, box):

    x1 = box.xmin
    y1 = box.ymin
    x2 = box.xmax
    y2 = box.ymax

    label = box.Class

    text = f'{label}' 
    color = (153,255,255)

    cv2.rectangle(img, (x1, y1), (x2, y2), color, 3)
    cv2.putText(img, text, (x1, y1),
                cv2.FONT_HERSHEY_SIMPLEX, 1, color, 2)

    return img
