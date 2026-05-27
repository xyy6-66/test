<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="java.util.List" %>
<%@ page import="entity.User" %>
<%@ page import="model.UserModel" %>
<html>
<head>
    <title>用户列表</title>
</head>
<body align="center">
    <h2>用户列表</h2>
    <table border="1" align="center">
        <tr>
            <th>ID</th>
            <th>用户名</th>
            <th>密码</th>
            <th>操作</th>
        </tr>
        <%
            UserModel um = new UserModel();
            List<User> list = um.findAll();
            if(list != null && list.size()>0){
                for(User u : list){
        %>
        <tr>
            <td><%=u.getId()%></td>
            <td><%=u.getName()%></td>
            <td><%=u.getPassword()%></td>
            <td>
                <%-- 重点：这里必须拼接 ?id= --%>
                <a href="update.jsp?id=<%=u.getId()%>">修改</a>
                <a href="deleteDo.jsp?id=<%=u.getId()%>">删除</a>
            </td>
        </tr>
        <%
                }
            }
        %>
    </table>
    <br>
    <a href="index.jsp">返回首页</a>
</body>
</html>