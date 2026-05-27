<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="model.UserModel" %>
<html>
<head>
    <title>删除结果</title>
</head>
<body>
<%
    int id = Integer.parseInt(request.getParameter("id"));
    UserModel um = new UserModel();
    boolean flag = um.delUser(id);

    if(flag){
        out.print("<h3 align='center'>删除成功！</h3>");
    }else{
        out.print("<h3 align='center'>删除失败！</h3>");
    }
%>
    <br>
    <div align="center"><a href="index.jsp">返回首页</a></div>
</body>
</html>