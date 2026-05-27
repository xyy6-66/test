<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="model.UserModel" %>
<html>
<body>
<%
    request.setCharacterEncoding("UTF-8");
    int id = Integer.parseInt(request.getParameter("id"));
    String name = request.getParameter("name");
    String password = request.getParameter("password");

    UserModel um = new UserModel();
    int n = um.update(id,name,password);

    if(n>0){
%>
        <script>alert("修改成功");location.href="allShow.jsp"</script>
<%
    }else{
%>
        <script>alert("修改失败");history.back()</script>
<%
    }
%>
</body>
</html>