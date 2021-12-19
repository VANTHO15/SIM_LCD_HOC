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
**1.AT**  Kiểm tra module có hoạt động không   
*Trả về: OK nếu hoạt động bình thường, báo lỗi hoặc 	không trả về nếu có lỗi xảy ra*  
**2.ATEx : Bật (x=1) hoặc tắt (x=0) chế độ phản hồi lệnh vừa gửi (nên tắt đi)**   
**3.AT+CPIN? :**  Kiểm tra Simcard     
*Trả về: +CPIN: READY OK (nếu tìm thấy simcard)*   
 **4.AT+CSQ :**  Kiểm tra chất lượng sóng       
 *Trả về: +CSQ: xx,0 OK (xx là chất lượng sóng, tối đa là 31)*   
**5.AT+COPS? :** Kiểm tra tên nhà mạng      
*Trả về: +COPS: 0,0,”Viettel Mobile” OK (nhận dạng được nhà mạng là Viettel Mobile)*   
*Trả về: +COPS: 0 (không thấy nhà mạng)*    
**6.ATD0702358769; :** Gọi điện cho số điện thoại 0702358769  
**7.ATA :** Chấp nhận cuộc gọi đến  
**8.ATH :** Hủy cuộc gọi  
**9.AT+CUSD=1,”*101##” :** Chuyển chế độ USD để tra số dư tài khoản  
**10.ATD*101#; :** Kiểm tra tài khoản  
*Trả về: +CUSD: 0, “Tai khoan goc cua quy khach la ….*  
**11.AT+CMGF=x :** Cấu hình tin nhắn **(x=0: DPU, x=1:dạng ký tự)**  
**12.AT+IPR=9600:** thay đổi tốc độ baud  
**13.AT+CNMI=2,x,0,0 :** Chọn *x=1*(chỉ báo vị trí lưu tin nhắn) hoặc *x=2* (hiển thị ra ngay nội dung tin nhắn)  
*Trả về: +CMTI: “SM”,3 (x=1)*  
*Trả về: +CMT: “+84938380171″,””,”17/07/30,14:48:09+28″ noidungtinnhan!*    
Sau đó **AT&W** : lưu các cái mình đã cấu hình!  
**14.AT+CMGR=x :** Đọc tin nhắn tại vị trí lưu x  
*Trả về: nội dung tin nhắn*  
**15.AT+CMGD=x :** Xóa tin nhắn được lưu ở vị trí x  
**16.AT+CMGS=”sodienthoai” :** Gửi tin nhắn cho sodienthoai, sau dòng lệnh này sẽ nhận được **‘>’** (mã HEX là 0x3C), bây giờ có thể nhập vào nội dung tin nhắn, nhập tiếp 0x1A để gửi tin nhắn đi hoặc 0x1B để hủy gửi tin nhắn.tin nhắn hay ox1a ox1b không cần <CR><LF>, 0x thay bằng $  































 





 












