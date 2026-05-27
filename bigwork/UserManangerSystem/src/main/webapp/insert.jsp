<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>新增用户信息</title>
</head>
<body align="center">
    <h2>新增用户信息</h2>
    <form action="insertDo.jsp" method="post">
        用户名：<input type="text" name="name"><br><br>
        密&nbsp;&nbsp;码：<input type="password" name="password"><br><br>
        <input type="submit" value="提交">
    </form>
    <br>
    <a href="index.jsp">返回首页</a>
</body>
</html>