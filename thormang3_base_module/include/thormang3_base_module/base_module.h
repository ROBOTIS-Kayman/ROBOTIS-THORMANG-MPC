/*******************************************************************************
 * Copyright (c) 2016, ROBOTIS CO., LTD.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of ROBOTIS nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

/*
 * ThorManipulation.h
 *
 *  Created on: 2016. 1. 18.
 *      Author: zerom
 */

#ifndef THORMANG3_BASE_MODULE_BASE_MODULE_H_
#define THORMANG3_BASE_MODULE_BASE_MODULE_H_

#include <map>
#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <ros/package.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float64.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Pose.h>
#include <boost/thread.hpp>

#include <Eigen/Dense>
#include <yaml-cpp/yaml.h>
#include <fstream>

#include "robotis_framework_common/motion_module.h"

#include "robotis_controller_msgs/StatusMsg.h"

#include "robotis_math/robotis_math.h"
#include "thormang3_kinematics_dynamics/kinematics_dynamics.h"

#include "base_module_state.h"

namespace thormang3
{

class BaseJointData
{
public:
  double position_;
  double velocity_;
  double effort_;

  int p_gain_;
  int i_gain_;
  int d_gain_;
};

class BaseJointState
{
public:
  BaseJointData curr_joint_state_[MAX_JOINT_ID + 1];
  BaseJointData goal_joint_state_[MAX_JOINT_ID + 1];
  BaseJointData fake_joint_state_[MAX_JOINT_ID + 1];
};

class BaseModule: public robotis_framework::MotionModule, public robotis_framework::Singleton<BaseModule>
{
public:
  BaseModule();
  virtual ~BaseModule();

  /* ROS Framework Functions */
  void initialize(const int control_cycle_msec, robotis_framework::Robot *robot);
  void process(std::map<std::string, robotis_framework::Dynamixel *> dxls, std::map<std::string, double> sensors);

  void stop();
  bool isRunning();

  /* ROS Topic Callback Functions */
  void initPoseMsgCallback(const std_msgs::String::ConstPtr& msg);

  /* ROS Calculation Functions */
  void initPoseTrajGenerateProc();

  void poseGenerateProc(Eigen::MatrixXd joint_angle_pose);
  void poseGenerateProc(std::map<std::string, double>& joint_angle_pose);

  /* Parameter */
  BaseModuleState  *base_module_state_;
  BaseJointState   *joint_state_;

private:
  void queueThread();

  void setCtrlModule(std::string module);
  void parseIniPoseData(const std::string &path);
  void publishStatusMsg(unsigned int type, std::string msg);
  void publishDoneMsg(const std::string done_msg);

  int           control_cycle_msec_;

  boost::thread queue_thread_;
  boost::thread tra_gene_tread_;

  // ros::Publisher      send_tra_pub_;
  ros::Publisher status_msg_pub_;
  ros::Publisher set_ctrl_module_pub_;
  ros::Publisher movement_done_pub_;

  std::map<std::string, int> joint_name_to_id_;

  bool has_goal_joints_;
  bool ini_pose_only_;
};

}

#endif /* THORMANG3_BASE_MODULE_BASE_MODULE_H_ */
