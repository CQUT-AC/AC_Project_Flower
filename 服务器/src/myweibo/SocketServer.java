package myweibo;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.Date;


public class SocketServer implements Runnable{
	sqlConn conn=new sqlConn();
	ServerSocket s = null;
    Socket socket = null;
    static BufferedReader br = null;
    static PrintWriter pw = null;

    public void Connect() throws Exception{
    	//设定服务端的端口号 
    	 s = new ServerSocket(8080);
         System.out.println("ServerSocket Start:"+s);
         while(true){
        	 socket = s.accept();
        	 System.out.println("已连接设备"+socket);
        	 //System.out.println("IP地址"+socket.getInetAddress().getHostAddress());
             //用于接收客户端发来的请求 
             br = new BufferedReader(new InputStreamReader(socket.getInputStream())); 
             //用于发送返回信息,可以不需要装饰这么多io流使用缓冲流时发送数据要注意调用.flush()方法
             pw = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())),true);  
//             System.out.println(str.getBytes());
         }
    }
    public void Close() throws Exception{
        br.close();
        pw.close();
        socket.close();
        s.close();
        System.out.println("Closed");
    }
	public String[] GetInfo(String time) throws Exception {
		pw.println("0");//0代表发出获取信息的命令
		pw.flush();
        String str = br.readLine();//str传过来的温度和湿度用空格隔开
        String[] parameter=str.split(" ");
        System.out.println("温度"+parameter[0]+"℃,湿度"+parameter[1]+"%");
        conn.saveParam(time, parameter[0]+"℃", parameter[1]+"%");//写入数据库
        SendWeibo.send(time,parameter[0], parameter[1]);//发微博
		Thread.sleep(1000);
		return parameter;
    }
	public void Watering() throws Exception{
		pw.println("1");//1代表发出浇水的命令
		pw.flush();
		String str = br.readLine();
		Thread.sleep(1000);
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		try {
			Connect();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
   	 }
	public static void main(String[] args) throws Exception {
		SocketServer server = new SocketServer();
		new Thread(server).start();//启动服务器
		SimpleDateFormat dt=new SimpleDateFormat("YYYY:MM:dd HH:mm:ss");
		String[] time=new String[2];
		String hour,second,minute;
		String temp;
		int count = 0;
		String [] paramter = new String[2];
		String sendTime = "08:00:00";
		String reciveTime = "20:00:00";
		while(true){
			//获取当前系统时间
			temp=dt.format(new Date());
			time=temp.split(" ");
			second = time[1].split(":")[2];
			//这里每隔一分钟统计一次连接时间，并且发送心跳包到客户端
			//用来检测客服端与服务器是否断开连接
			if(second.equals("10")){
				try{
					count++;
					server.socket.sendUrgentData(2);
					System.out.println("已连上"+count+"分钟");
					pw.println("2");//0代表发出获取信息的命令
					pw.flush();
				}
				catch(Exception e)
				{
					count = 0;
					System.out.println("还没有客户端连上");
				}
				Thread.sleep(1000);
			}
			//如果到了发送微博的时间
			if(time[1].equals(sendTime)){
				try{
				paramter=server.GetInfo(temp);//写入数据库并发微博
				}catch(Exception e){}
			}
			//如果到了获取微博评论并判断是否浇水的时间
			if(time[1].equals(reciveTime)){
				String weibo = "我是AC,现在是"+time[0]+" "+sendTime+",当前的温度"+paramter[0]+"℃,湿度"+paramter[1]+"%,今天是否要浇水呢？";
				//当获取评论的yes的个数大于等于no的个数时就浇水
				try {
					int[] result=GetCommentById.result(weibo);
					if(result[0]>=result[1]){
						server.Watering();
						System.out.println("watering");
					}
					sqlConn conn=new sqlConn();
					conn.saveComments(time[0]+" "+sendTime, result[0], result[1]);
				} catch (Exception e) {
					// TODO: handle exception
					System.out.println("正在拼命重启。。。");
				}
			}
		}
	}
}
