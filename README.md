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
![image](https://user-images.githubusercontent.com/86427371/146667075-956506a8-e573-4e86-aab9-34002073df6e.png)  
Bây giờ chúng ta sẽ thực hiện các tập lệnh AT trên Hercules. Mọi người kết nối như hình trên.  
**1.AT**  Kiểm tra module có hoạt động không 
![image](https://user-images.githubusercontent.com/86427371/146667151-12a66279-c030-4960-b32d-fa865fd9f2a0.png)  
*Trả về: **OK** nếu hoạt động bình thường, báo lỗi hoặc 	không trả về nếu có lỗi xảy ra* 

**2.ATEx**  : Bật (x=1) hoặc tắt (x=0) chế độ phản hồi lệnh vừa gửi (nên tắt đi) 
![image](https://user-images.githubusercontent.com/86427371/146667194-552c8fcf-8a35-416e-80b9-c07c6fdf09ed.png)  

**3.AT+CPIN?** :  Kiểm tra Simcard  
![image](https://user-images.githubusercontent.com/86427371/146667209-8e068cae-af6a-4e52-8847-6ee3dc913c83.png)  
*Trả về: **+CPIN: READY OK** (nếu tìm thấy simcard)*  

 **4.AT+CSQ** :  Kiểm tra chất lượng sóng  
 ![image](https://user-images.githubusercontent.com/86427371/146667230-92eceaad-6116-49de-979c-08b23b275660.png)  
*Trả về: **+CSQ: xx,0 OK** (xx là chất lượng sóng, tối đa là 31)*  

**5.AT+COPS?** : Kiểm tra tên nhà mạng
![image](https://user-images.githubusercontent.com/86427371/146667248-ccd9f4ae-311d-4055-ad13-144651d357e1.png)    
*Trả về: **+COPS: 0,0,”Viettel Mobile” OK** (nhận dạng được nhà mạng là Viettel Mobile)*   
*Trả về: **+COPS: 0** (không thấy nhà mạng)*  

**6.ATD0702358769;** Gọi điện cho số điện thoại 0702358769    
![image](https://user-images.githubusercontent.com/86427371/146667265-2d30be96-514d-4403-b8ab-e0001c2aac7b.png)    

**7.ATA**: Chấp nhận cuộc gọi đến   
![image](https://user-images.githubusercontent.com/86427371/146667278-0d846327-a1df-4c94-a4ba-74bb7a8900d8.png)    

**8.ATH**: Hủy cuộc gọi    
![image](https://user-images.githubusercontent.com/86427371/146667291-d091f1c4-67d1-425c-a0dd-6e296ed46ac8.png)  

9.AT+CUSD=1,”*101##” : Chuyển chế độ USD để tra số dư tài khoản  
![image](https://user-images.githubusercontent.com/86427371/146667305-611adb94-3e2c-4794-9bc6-a7a51ecc2c05.png)    
![image](https://user-images.githubusercontent.com/86427371/146667322-135f46dd-3c35-4962-853f-0daf431c3474.png)  

**10.ATD*101#;** :Kiểm tra tài khoản  
![image](https://user-images.githubusercontent.com/86427371/146667370-27f4f071-e0b7-4e70-bf55-d5e8ae47d6d3.png)  
*Trả về: +CUSD: 0, “Tai khoan goc cua quy khach la ….*   

**11.AT+CMGF=x** : Cấu hình tin nhắn **(x=0: DPU, x=1:dạng ký tự)**  
![image](https://user-images.githubusercontent.com/86427371/146667467-2776ec57-e53c-4f20-835e-bdde09f8910f.png)  
Ở đây mình cấu hình tin nhắn ở dạng kí tự nhé !  

**12.AT+IPR=9600** : thay đổi tốc độ baud  
![image](https://user-images.githubusercontent.com/86427371/146667507-2d948a42-4bc3-4d89-9d4c-0123a9bb4417.png)  
Mọi người nên chọn tốc độ baud là 9600  

**13.AT+CNMI=2,x,0,0** : Chọn *x=1*(chỉ báo vị trí lưu tin nhắn) hoặc *x=2* (hiển thị ra ngay nội dung tin nhắn)  
![image](https://user-images.githubusercontent.com/86427371/146667538-79976c19-2318-49dc-a9b7-818d56b85096.png)  
*Trả về: +CMTI: “SM”,3 (x=1)*  
*Trả về: +CMT: “+84938380171″,””,”17/07/30,14:48:09+28″ noidungtinnhan!*  

Sau đó **AT&W** : lưu các cái mình đã cấu hình!  
![image](https://user-images.githubusercontent.com/86427371/146667567-3c3bd99f-1b16-436b-803a-3c49bafc48f2.png)  
Cái này khá quan trọng vì khi chúng ta đã cấu hình xong khi tắt Hercules rồi bật lại thì những  cái chúng ta đã cấu hình vẫn còn lưu.  
**14.AT+CMGR=x** : Đọc tin nhắn tại vị trí lưu x  
![image](https://user-images.githubusercontent.com/86427371/146667616-28df0b76-afcf-4c16-921b-7db562f156b0.png)  
*Trả về: nội dung tin nhắn*  
**15.AT+CMGD=x** : Xóa tin nhắn được lưu ở vị trí x  
![image](https://user-images.githubusercontent.com/86427371/146667702-23c505f5-9d9d-4715-b68e-d6a141b3b912.png)  

**16.AT+CMGS=”sodienthoai”** : Gửi tin nhắn cho sodienthoai  
![image](https://user-images.githubusercontent.com/86427371/146667743-db217ff2-e9ce-48f9-86f0-4074759e1b68.png)  
Sau dòng lệnh này sẽ nhận được **‘>’** (mã HEX là 0x3C), bây giờ có thể nhập vào nội dung tin nhắn, nhập tiếp **0x1A** để gửi tin nhắn đi hoặc **0x1B** để hủy gửi tin nhắn.Tới bước này chúng ta không cần gõ dòng lệnh <CR><LF> nữa nhé!  
 Vì trên Hercules không gửi được 0x1A nên chúng ta gõ $1A nhé !
 ![image](https://user-images.githubusercontent.com/86427371/146668182-8e75da61-408e-449c-b270-1cccafd87ff6.png)  
 
 
Như vậy chúng ta có thể gọi và gửi tin nhắn từ MODULE SIM800L.
Cảm ơn mọi người đã quan tâm và theo dõi   
Chúc mọi người thành công !!! 
































 





 












