package dbutil;

import java.sql.Connection;
import java.sql.DriverManager;

public class Dbconn {
    private static final String URL = "jdbc:mysql://localhost:3306/mydatabase?useSSL=false&serverTimezone=UTC&characterEncoding=UTF-8";
    private static final String USER = "root";
    // 这里改成你自己的MySQL密码
    private static final String PWD = "123456";

    static {
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static Connection getConnection() {
        Connection conn = null;
        try {
            conn = DriverManager.getConnection(URL, USER, PWD);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return conn;
    }
}