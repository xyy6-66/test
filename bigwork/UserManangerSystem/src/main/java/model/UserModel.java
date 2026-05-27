package model;

import dbutil.Dbconn;
import entity.User;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.List;

public class UserModel {

    public List<User> findAll() {
        List<User> list = new ArrayList<>();
        Connection conn = null;
        PreparedStatement pstmt = null;
        ResultSet rs = null;
        try {
            conn = Dbconn.getConnection();
            String sql = "SELECT id,name,password FROM user";
            pstmt = conn.prepareStatement(sql);
            rs = pstmt.executeQuery();
            while (rs.next()) {
                User u = new User();
                u.setId(rs.getInt("id"));
                u.setName(rs.getString("name"));
                u.setPassword(rs.getString("password"));
                list.add(u);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return list;
    }

    public int insert(String name, String password) {
        int n = 0;
        Connection conn = null;
        PreparedStatement pstmt = null;
        try {
            conn = Dbconn.getConnection();
            String sql = "INSERT INTO user(name,password) VALUES (?,?)";
            pstmt = conn.prepareStatement(sql);
            pstmt.setString(1, name);
            pstmt.setString(2, password);
            n = pstmt.executeUpdate();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return n;
    }

    public User findById(int id) {
        User u = null;
        Connection conn = null;
        PreparedStatement pstmt = null;
        ResultSet rs = null;
        try {
            conn = Dbconn.getConnection();
            String sql = "SELECT id,name,password FROM user WHERE id=?";
            pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, id);
            rs = pstmt.executeQuery();
            if (rs.next()) {
                u = new User();
                u.setId(rs.getInt("id"));
                u.setName(rs.getString("name"));
                u.setPassword(rs.getString("password"));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return u;
    }

    public int update(int id, String name, String password) {
        int n = 0;
        Connection conn = null;
        PreparedStatement pstmt = null;
        try {
            conn = Dbconn.getConnection();
            String sql = "UPDATE user SET name=?,password=? WHERE id=?";
            pstmt = conn.prepareStatement(sql);
            pstmt.setString(1, name);
            pstmt.setString(2, password);
            pstmt.setInt(3, id);
            n = pstmt.executeUpdate();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return n;
    }

    public int delete(int id) {
        int n = 0;
        Connection conn = null;
        PreparedStatement pstmt = null;
        try {
            conn = Dbconn.getConnection();
            String sql = "DELETE FROM user WHERE id=?";
            pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, id);
            n = pstmt.executeUpdate();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return n;
    }
}