<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="model.UserModel" %>
<%
    request.setCharacterEncoding("UTF-8");
    String name = request.getParameter("name");
    String password = request.getParameter("password");

    UserModel um = new UserModel();
    int rows = um.insert(name, password);

    if (rows > 0) {
        response.sendRedirect("allShow.jsp");
    } else {
        out.print("新增失败");
    }
%>