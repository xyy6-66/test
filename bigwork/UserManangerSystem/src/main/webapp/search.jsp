<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>查询用户</title>
</head>
<body>
    <h3 align="center">根据ID查询用户</h3>
    <form action="searchDo.jsp" method="post" align="center">
        用户ID：<input type="number" name="id"><br/><br/>
        <input type="submit" value="查询">
    </form>
    <br>
    <div align="center"><a href="index.jsp">返回首页</a></div>
</body>
</html>