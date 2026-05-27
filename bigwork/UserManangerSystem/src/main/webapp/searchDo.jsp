<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="model.UserModel" %>
<%@ page import="entity.User" %>
<html>
<head>
    <title>查询结果</title>
</head>
<body>
<%
    int id = Integer.parseInt(request.getParameter("id"));
    UserModel um = new UserModel();
    User u = um.getById(id);
    if(u != null){
%>
    <table border="1" align="center">
        <tr>
            <td>ID</td>
            <td><%=u.getId()%></td>
        </tr>
        <tr>
            <td>用户名</td>
            <td><%=u.getUsername()%></td>
        </tr>
        <tr>
            <td>密码</td>
            <td><%=u.getPassword()%></td>
        </tr>
        <tr>
            <td>年龄</td>
            <td><%=u.getAge()%></td>
        </tr>
    </table>
<%
    }else{
        out.print("<h3 align='center'>未找到该用户</h3>");
    }
%>
    <br>
    <div align="center"><a href="index.jsp">返回首页</a></div>
</body>
</html>