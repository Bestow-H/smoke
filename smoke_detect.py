import cv2
import serial
import struct
from ultralytics import YOLO


ser = serial.Serial()
# 配置串口参数
def port_open_recv(ser):
    ser.port = '/dev/ttyUSB0' # 串口号
    ser.baudrate = 9600 # 波特率
    ser.bytesize = 8 # 数据位
    ser.stopbits = 1 # 停止位
    ser.parity = "N" # 奇偶校验位
    ser.open() # 打开串口
    if ser.isOpen():
        print("串口打开成功！")
    else:
        print("串口打开失败！")

# 关闭串口
def port_close(ser):
    ser.close()
    if not ser.isOpen():
        print("串口关闭成功！")

# 发送数据
def send(send_data,ser):
    head_pack:bytes=b'\xCD'
    footer:bytes=b'\xDC'
    data=struct.pack('<h',send_data)
    data_send=head_pack+data+footer
    if ser.isOpen():
        ser.write(data_send) # 发送数据
        print("发送成功", data_send)
    else:
        print("发送失败！")





# Load the YOLO model
model = YOLO("/home/jetson/best.engine")

# Open the camera
cap = cv2.VideoCapture(0)

# Get the video frame size and frame rate
frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
fps = int(cap.get(cv2.CAP_PROP_FPS))

# Define the codec and create a VideoWriter object to output the processed video
# output_path = "/home/jetson/ultralytics/ultralytics/output/06.orange_camera_usb.mp4"
# fourcc = cv2.VideoWriter_fourcc(*'mp4v')
# out = cv2.VideoWriter(output_path, fourcc, fps, (frame_width, frame_height))

port_open_recv(ser)
# Loop through the video frames
while cap.isOpened():
    success, frame = cap.read()

    if success:
        # Run YOLO inference on the frame
        results = model(frame)
        selected_boxes = []  # 存储候选框

        # 获取检测目标坐标（新增部分） #
        # 遍历所有检测结果
        for result in results:
            # 遍历当前结果中的所有边界框
            for box in result.boxes:
                if(float(box.conf[0])>0.4):
                # 获取边界框坐标（xyxy格式）
                    coordinates = box.xyxy[0].tolist()
                    x1, y1, x2, y2 = coordinates
                    if(class_id==0):
                        selected_boxes.append( (x1, box) )
                            # print(len(results.boxes))
                            # 计算中心点坐标
                        center_x = int((x1 + x2) / 2)
                        center_y = int((y1 + y2) / 2)
                            
                            # 打印坐标信息
                        print(f"目标边界框坐标: 左上({x1:.1f}, {y1:.1f}), 右下({x2:.1f}, {y2:.1f})")
                        print(f"中心点坐标: ({center_x}, {center_y})")
                            
                            # 如果需要显示类别和置信度
                        class_id = int(box.cls[0])
                        confidence = float(box.conf[0])
                        print(f"类别ID: {class_id}, 置信度: {confidence:.2f}\n")
                
                    
                    
                        
        # # 第二阶段：选择最左侧框并发送
        if selected_boxes:
            # 按x1升序排列，取第一个
            selected_boxes.sort(key=lambda x: x[0])
            min_x_box = selected_boxes[0][1]
            
            # 计算中心坐标
            x1, y1, x2, y2 = min_x_box.xyxy[0].tolist()
            center_x = int((x1 + x2) / 2)
            
            # 发送坐标
            send(round(center_x), ser)
            print(f"发送最左侧坐标: {center_x}")
                    
        # 可视化结果
        annotated_frame = results[0].plot()

        # Write the annotated frame
        # out.write(annotated_frame)

        # Display the frame
        cv2.imshow("YOLO Inference", cv2.resize(annotated_frame, (640, 480)))

        if cv2.waitKey(1) & 0xFF == ord("q"):
            break
    

port_close(ser)
cap.release()
# out.release()
cv2.destroyAllWindows()
