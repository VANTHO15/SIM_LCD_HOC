# MODULE SIM800L
## I.Giới thiệu MODULE SIM800L
![image](https://user-images.githubusercontent.com/86427371/146665885-047bd1b6-f462-4f83-b7e4-dfc39415e1e3.png)
![image](https://user-images.githubusercontent.com/86427371/146665892-799747d0-0938-4859-9223-51c0f5bec8f3.png)  
 Chuẩn bị mạch cấp nguồn tốt , khuyên dùng với mạch có ngõ ra từ 2A trở lên. Thực tế thì module sim chỉ cần tối đa cỡ 500mA/4V thôi nhưng đa số các mạch nguồn đều không đảm bảo đủ công suất và khi khởi động cũng tốn khá nhiều nguồn nên chúng ta nên cấp dư hơn nhiều cái nó cần để đảm bảo hoạt động trơn tru.  
 Với các bạn sử dụng module sim có sẵn mạch hạ áp thì cứ sử dụng đúng thông số đầu vào trên mạch là ok. Thông thường đều hỗ trợ với nguồn 12V/2A hoặc 5V/2A
Với các mạch chỉ ra chân và không có sẵn mạch nguồn, nếu bạn có sẵn nguồn cấp 5V thì nên dùng mạch hạ áp LDO MIC29302, nếu bạn có nguồn 9V,12V hoặc 24V thì nên sử dụng mạch hạ áp LM2596-ADJ ( nên chỉnh áp ra trước khi cấp vào mạch sim)  
![image](https://user-images.githubusercontent.com/86427371/146665990-cdb265e9-e8e9-4844-acc5-7facd535bb12.png)  
Một số module sim sử dụng diode hạ áp từ 5V xuống 4V3 để nuôi mạch thì bạn nên lưu ý cấp đúng nguồn 5V cho loại này, tốt nhất nên sử dụng mạch **LM2596** hạ áp xuống 5V để nuôi mạch, và nên mắc thêm **tụ bù 1000uF** để đảm bảo hoạt động ổn định.
## II.Phần cứng  
![image](https://user-images.githubusercontent.com/86427371/146666138-612ffd35-5f2b-4d40-8965-a35ec3338373.png)  
![image](https://user-images.githubusercontent.com/86427371/146666168-68ca2048-0175-463b-ae57-9df817011e56.png)  
Những nơi sóng mạnh thì có thể không cần đến anten  
Có 3 loại anten : anten helical đi kèm, anten PCB, anten qua cổng IPX- SMA
![image](https://user-images.githubusercontent.com/86427371/146666256-a009f61f-3f8d-47cc-bc93-95de498854de.png)  
Sử dụng module điều chỉnh hạ áp qua LM2596 điều chỉnh về 4.2V  
Kết nối như trên hình!  
TX và RX phải nối chéo với nhau giữa các module, đồng  nối mass chung  
Nếu không có CH340 có thể dùng USB TTL  
## III.Tập Lệnh AT  
**1.AT Kiểm tra module có hoạt động không**  
*Trả về: OK nếu hoạt động bình thường, báo lỗi hoặc 	không trả về nếu có lỗi xảy ra  













