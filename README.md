![image](https://user-images.githubusercontent.com/56969447/146633030-6b027dad-3b5f-4aea-8f60-2e34cdd7b8f6.png)
#
![image](https://user-images.githubusercontent.com/56969447/146633049-cdd8196e-d764-4925-adf8-20bbe4fd7207.png)
#
![image](https://user-images.githubusercontent.com/56969447/146633061-65f4d5dd-6212-4060-a15c-8a3bcc47bd25.png)
#
![image](https://user-images.githubusercontent.com/56969447/146633066-fc70a7c8-ce8e-4934-9fbd-eff989eac3a0.png)
#
![image](https://user-images.githubusercontent.com/56969447/146633097-f827e846-5b39-46fc-95e0-03ed28444787.png)
#
# Cấp nguồn 4.2V và thêm tụ hóa 1000uF để nguồn ổn định hơn.
# Ban đầu cấp nguồn thì led nhấp nháy 1 giây, đợi 30s thì sim kết nối với modune Sim và led sẽ nháy 3 giây 1 lần
#AT : Kiểm tra module có hoạt động không
	Trả về: OK nếu hoạt động bình thường, báo lỗi hoặc 	không trả về nếu có lỗi xảy ra
ATEx : Bật (x=1) hoặc tắt (x=0) chế độ phản hồi lệnh vừa gửi (nên tắt đi)
AT+CPIN? : Kiểm tra Simcard
	Trả về: +CPIN: READY OK (nếu tìm thấy simcard)
AT+CSQ : Kiểm tra chất lượng sóng
	Trả về: +CSQ: xx,0 OK (xx là chất lượng sóng, tối đa là 31)
AT+COPS? : Kiểm tra tên nhà mạng
	Trả về: +COPS: 0,0,”Viettel Mobile” OK (nhận dạng được nhà mạng là Viettel Mobile)
	Trả về: +COPS: 0 (không thấy nhà mạng)
![image](https://user-images.githubusercontent.com/56969447/146633294-07a91dd4-1242-4eb6-bac3-9ca318014902.png)

#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
#
