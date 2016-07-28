package myweibo;
import weibo4j.Comments;
import weibo4j.Timeline;
import weibo4j.Weibo;
import weibo4j.examples.oauth2.Log;
import weibo4j.model.Comment;
import weibo4j.model.CommentWapper;
import weibo4j.model.Status;
import weibo4j.model.WeiboException;

public class SendWeibo {
	public static void send(String time,String temperature,String wet) 
	{
		String access_token = "2.00pcH93G04Y6yT95bf35d40909mB1E";
		String weibo = "我是AC,现在是"+time+",当前的温度"+temperature+"℃,湿度"+wet+"%,今天是否要浇水呢？";
		Timeline tm = new Timeline(access_token);
		try
		{
			Status status = tm.updateStatus(weibo);
			Log.logInfo(status.toString());
		}
		catch (WeiboException e)
		{
			e.printStackTrace();
		}			 		
	}

}
