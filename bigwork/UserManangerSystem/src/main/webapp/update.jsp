<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="model.UserModel" %>
<%@ page import="entity.User" %>
<html>
<head>
    <title>修改用户</title>
</head>
<body align="center">
<%
    String idStr = request.getParameter("id");
    // 增加判断，防止 null 报错
    if(idStr == null){
        out.print("请从列表点击修改");
        return;
    }
    int id = Integer.parseInt(idStr);
    UserModel um = new UserModel();
    User u = um.findById(id);
%>
    <h2>修改用户</h2>
    <form action="updateDo.jsp" method="post">
        <input type="hidden" name="id" value="<%=u.getId()%>">
        用户名：<input type="text" name="name" value="<%=u.getName()%>"><br><br>
        密码：<input type="text" name="password" value="<%=u.getPassword()%>"><br><br>
        <input type="submit" value="提交修改">
    </form>
    <br>
    <a href="allShow.jsp">返回列表</a>
</body>
</html>