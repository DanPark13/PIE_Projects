import ipyvolume as ipv
import numpy as np
from scipy.spatial.transform import Rotation
from IPython.core.debugger import set_trace

# Initialize plotting
x, y, z = np.zeros((3, 1))
fig = ipv.figure()
scatter = ipv.scatter(x, y, z)
ipv.show()

# Initialize data matrix
mat_data = np.transpose(np.array([scatter.x, scatter.y, scatter.z]))

# Define plotting callback
def plot_callback(data):
    global mat_data, scatter
    
    # ======== Preprocess and cleanup input data ========
    # Extract comma separated values into vector
    vals = data.split(",")
    try:
        vals = list(map(float, vals[0:3]))
    except:
        return
    vals = np.array(vals)
    
    if np.size(vals, 0) == 3:
        pan_offset = 0
        tilt_offset = -45
        
        deg_pan = vals[0] + pan_offset # Pan angle in degrees
        deg_tilt = vals[1] + tilt_offset # Tilt angle in degrees
        range_measurement = vals[2] # Range measurement in inches
        
        if range_measurement > 50 or range_measurement < 10 / 2.54:
            return
        
        # ==== Convert servo angles and distance to pose relative to servo =====
        def SE3(mat_R, x, y, z): # SE3 Pose matrix creation function
            mat_out = np.eye(4)
            mat_out[0:3, 0:3] = mat_R
            mat_out[0:3, 3] = np.array([x, y, z])
            
            return mat_out
        
        # Rotation from lower servo to upper servo (along the lower arm)
        R_lower_arm = np.array([[0,  0, 1.0],
                                [1.0, 0, 0],
                                [0, 1.0, 0]
                               ])
        pose_lower_arm = SE3(R_lower_arm, 1.735, 0, 2.437)
        pose_upper_arm = SE3(np.eye(3), 2.215, 0, -1.594)
        
        # Servo joint angle poses
        R_joint_pan = Rotation.from_euler('z', deg_pan, degrees=True).as_matrix()
        R_joint_tilt = Rotation.from_euler('z', deg_tilt, degrees=True).as_matrix()
        pose_joint_pan = SE3(R_joint_pan, 0, 0, 0)
        pose_joint_tilt = SE3(R_joint_tilt, 0, 0, 0)
        
        # Combine all transformations from along the arm into one transformation matrix
        mat_transform = pose_joint_pan @ pose_lower_arm @ pose_joint_tilt @ pose_upper_arm
        
        # Measurement pose - straight line along x axis of sensor
        pose_measurement = SE3(np.eye(3), range_measurement, 0, 0)
        
        # Transform measurement pose from sensor frame into world frame
        world_pose_measurement = mat_transform @ pose_measurement 
        
         # Save the position in the world frame
        position_world = world_pose_measurement[0:3, 3]
        
        # Save the incoming data
        mat_data = np.append(mat_data, np.array([position_world]), 0)
        scatter.x = np.append(scatter.x, position_world[0])
        scatter.y = np.append(scatter.y, position_world[1])
        scatter.z = np.append(scatter.z, position_world[2])

        # ======== Update limits of figure based on max and min values ========
        v_mins = np.amin(mat_data, axis=0)
        v_ranges = np.amax(mat_data, axis=0) - v_mins # Vector of x y z ranges
        max_range = np.max(v_ranges) # Max range of all x y z
        mat_lims = np.transpose(np.array([v_mins, v_mins + max_range]))

        ipv.xlim(mat_lims[0, 0], mat_lims[0, 1])
        ipv.ylim(mat_lims[1, 0], mat_lims[1, 1])
        ipv.zlim(mat_lims[2, 0], mat_lims[2, 1])
        
# Listen on serial
from serial_listen import serial_listen
serial_listen(plot_callback, arduino_port="/dev/ttyACM2")
