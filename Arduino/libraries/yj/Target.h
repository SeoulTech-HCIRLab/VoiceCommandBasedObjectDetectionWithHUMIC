#ifndef _ROS_yj_Target_h
#define _ROS_yj_Target_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace yj
{

  class Target : public ros::Msg
  {
    public:
      typedef int16_t _target_type;
      _target_type target;
      typedef int16_t _degree_type;
      _degree_type degree;
      typedef int16_t _direction_type;
      _direction_type direction;
      typedef int16_t _depth_type;
      _depth_type depth;
      typedef int16_t _xc_type;
      _xc_type xc;
      typedef int16_t _yc_type;
      _yc_type yc;
      typedef int16_t _start_flag_type;
      _start_flag_type start_flag;

    Target():
      target(0),
      degree(0),
      direction(0),
      depth(0),
      xc(0),
      yc(0),
      start_flag(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_target;
      u_target.real = this->target;
      *(outbuffer + offset + 0) = (u_target.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_target.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->target);
      union {
        int16_t real;
        uint16_t base;
      } u_degree;
      u_degree.real = this->degree;
      *(outbuffer + offset + 0) = (u_degree.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_degree.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->degree);
      union {
        int16_t real;
        uint16_t base;
      } u_direction;
      u_direction.real = this->direction;
      *(outbuffer + offset + 0) = (u_direction.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_direction.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->direction);
      union {
        int16_t real;
        uint16_t base;
      } u_depth;
      u_depth.real = this->depth;
      *(outbuffer + offset + 0) = (u_depth.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_depth.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->depth);
      union {
        int16_t real;
        uint16_t base;
      } u_xc;
      u_xc.real = this->xc;
      *(outbuffer + offset + 0) = (u_xc.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_xc.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->xc);
      union {
        int16_t real;
        uint16_t base;
      } u_yc;
      u_yc.real = this->yc;
      *(outbuffer + offset + 0) = (u_yc.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_yc.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->yc);
      union {
        int16_t real;
        uint16_t base;
      } u_start_flag;
      u_start_flag.real = this->start_flag;
      *(outbuffer + offset + 0) = (u_start_flag.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_start_flag.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->start_flag);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_target;
      u_target.base = 0;
      u_target.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_target.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->target = u_target.real;
      offset += sizeof(this->target);
      union {
        int16_t real;
        uint16_t base;
      } u_degree;
      u_degree.base = 0;
      u_degree.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_degree.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->degree = u_degree.real;
      offset += sizeof(this->degree);
      union {
        int16_t real;
        uint16_t base;
      } u_direction;
      u_direction.base = 0;
      u_direction.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_direction.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->direction = u_direction.real;
      offset += sizeof(this->direction);
      union {
        int16_t real;
        uint16_t base;
      } u_depth;
      u_depth.base = 0;
      u_depth.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_depth.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->depth = u_depth.real;
      offset += sizeof(this->depth);
      union {
        int16_t real;
        uint16_t base;
      } u_xc;
      u_xc.base = 0;
      u_xc.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_xc.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->xc = u_xc.real;
      offset += sizeof(this->xc);
      union {
        int16_t real;
        uint16_t base;
      } u_yc;
      u_yc.base = 0;
      u_yc.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_yc.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->yc = u_yc.real;
      offset += sizeof(this->yc);
      union {
        int16_t real;
        uint16_t base;
      } u_start_flag;
      u_start_flag.base = 0;
      u_start_flag.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_start_flag.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->start_flag = u_start_flag.real;
      offset += sizeof(this->start_flag);
     return offset;
    }

    const char * getType(){ return "yj/Target"; };
    const char * getMD5(){ return "284a96bcffbe7d77a87e96b4c19b5205"; };

  };

}
#endif
