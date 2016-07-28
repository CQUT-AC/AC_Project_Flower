package myweibo;

import java.sql.*;

public class sqlConn {
	Statement stat;
	Connection conn;
	ResultSet rs;
	public sqlConn() {
		String driver = "com.mysql.jdbc.Driver";//加载驱动
		String dbUrl = "jdbc:mysql://localhost:3306/ac_data?characterEncoding=utf8";//连接服务器和数据库
		String userName = "root";
		String userPwd = "";
		try {
			Class.forName(driver);
			conn = DriverManager.getConnection(dbUrl, userName, userPwd);
			stat = conn.createStatement();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}
	/*
	 * 保存当前花的温度和湿度
	 * */
	public void saveParam(String time,String temperature,String wet) {
		String sql = "insert into ac_flower(Time,Temperature,Wet) " +
			"values('"+time+"','"+temperature+"','"+wet+"')";
		try {
			stat.execute(sql);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}
	public boolean getParam(String time){
		String sql = "select * from ac_flower where Time='"+time+"'";
		boolean flag = false;
		try {
			rs=stat.executeQuery(sql);
			while(rs.next()){
				flag = true;
			}
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return flag;
	}
	/*
	 * 保存评论情况及浇水情况
	 * */
	public void saveComments(String time,int yes,int no) {
		String sql = "update ac_flower set Comments_Yes=" +yes+",Comments_No="+no+",Comments_Water='30ml' where Time='"+time+"'";
		try {
			stat.execute(sql);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}
	/*
	 * 判断是否该浇花
	 * */
	public boolean judge(String time) {
		String sql = "select * from ac_flower where Time like '"+time+"%'";
		try {
			rs = stat.executeQuery(sql);
			rs.next();
			int yes = rs.getInt("Comments_Yes");
			int no = rs.getInt("Comments_No");
			rs.close();
			if(yes>=no)
				return true;
			else
				return false;
		} catch (Exception e) {
			// TODO: handle exception
		}
		return false;
	}
}
