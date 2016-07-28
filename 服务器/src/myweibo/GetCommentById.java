package myweibo;  
  
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.ArrayList;
import java.util.List;

import weibo4j.Comments;  
import weibo4j.Weibo;  
import weibo4j.examples.oauth2.Log;  
import weibo4j.model.Comment;  
import weibo4j.model.CommentWapper;  
import weibo4j.model.WeiboException;  

public class GetCommentById {  
	 public static ArrayList<String> list = new ArrayList<String>();
	
	 public static void main(String[] args) {
		 String wb = "时间2016:07:26 14:00:50    温度23.93    湿度41.59";
	    	
	        String access_token = "2.00pcH93G04Y6yT95bf35d40909mB1E";  
	        String com = comment(access_token);
	        
	        System.out.println("微博"+"“ "+wb+" ”"+"的评论如下：");
	        dealstring(com,wb);//获取评论信息并将评论信息添加到list中
	        for(int i=0;i<list.size();i++)
	        {
	        	System.out.println(list.get(i));
	        }
	}
    public static int[] result(String wb)
    {  
        String access_token = "2.00pcH93G04Y6yT95bf35d40909mB1E";  
        String com = comment(access_token);
        
        System.out.println("微博"+"“ "+wb+" ”"+"的评论如下：");
        dealstring(com,wb);//获取评论信息并将评论信息添加到list中
        
        int result[]=new int[2];//0下标代表yes，1下标代表no
        result[0]=0;
        result[1]=0;
        for(int i=0;i<list.size();i++)
        {
        	System.out.println(list.get(i));
        	if(((list.get(i)).toLowerCase()).equals("yes"))
        		result[0]++;
        	else if(((list.get(i)).toLowerCase()).equals("no"))
        		result[1]++;
        }
        return result;
    }
    //通过API获取微博的评论信息、	
    public static String comment(String access_token)
    {
    	String str = "";
    	Comments cm = new Comments(access_token);
		try
		{
			CommentWapper comment = cm.getCommentToMe();
			Log.logInfo(comment.toString());
			str = comment.toString();
		}
		catch (WeiboException e)
		{}
		return str;
    }
	//line 是待处理的字符串， weiboStr 是发布的最新微博内容
	public static void dealstring(String line, String weiboStr)
	{
		//截取在后一条评论前面的所有字段
		int index = line.lastIndexOf(weiboStr);
		if(-1 == index) return;
		line = line.substring(0, index+weiboStr.length()+10);
		//保存评论
		String comment = "";
		String weibo = "";
		while(true)
		{
			//通过"text="找到评论
			//此处是先截取评论
			index=line.indexOf("text=");
			if(-1 == index) return;
			line = line.substring(index+5);
			index=line.indexOf(", source");
			if(-1 == index) return;
			comment = line.substring(0,index);
			//此处是是截取微博
			index=line.indexOf("text=");
			if(-1 == index) return;
			line = line.substring(index+5);
			index=line.indexOf(", source");
			if(-1 == index) return;
			weibo = line.substring(0,index);
			//判断是判断得到的微博是否是指定的微博，如果是该微博的评论就保存到list中
			if(weibo.equals(weiboStr))
			{
				list.add(comment);
			}
		}
	}
    
}
