-- MySQL dump 10.13  Distrib 5.7.10, for osx10.9 (x86_64)
--
-- Host: localhost    Database: kq
-- ------------------------------------------------------
-- Server version	5.7.10

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `kq_day`
--

DROP TABLE IF EXISTS `kq_day`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kq_day` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` date NOT NULL COMMENT '日期',
  `name` varchar(10) NOT NULL COMMENT '姓名',
  `mac` varchar(20) NOT NULL COMMENT 'MAC 地址',
  `work_on_time` time NOT NULL COMMENT '工作开始时间',
  `work_off_time` time NOT NULL COMMENT '工作结束时间',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=49 DEFAULT CHARSET=utf8 COMMENT='日考勤';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `kq_day`
--

LOCK TABLES `kq_day` WRITE;
/*!40000 ALTER TABLE `kq_day` DISABLE KEYS */;
INSERT INTO `kq_day` VALUES (26,'2016-08-27','Dell旧服务器','d4:ae:52:75:c8:e6','00:00:03','23:57:04'),(25,'2016-08-26','刘德山','80:e6:50:00:ec:26','20:45:04','21:21:04'),(23,'2016-08-26','web服务器','a4:ba:db:4d:19:43','20:45:04','23:57:04'),(24,'2016-08-26','刘德山phone','cc:29:f5:8a:88:68','20:48:03','21:21:04'),(22,'2016-08-26','Dell新服务器','24:6e:96:0c:8e:3c','20:45:04','23:57:04'),(21,'2016-08-26','Dell旧服务器','d4:ae:52:75:c8:e6','20:45:04','23:57:04'),(27,'2016-08-27','Dell新服务器','24:6e:96:0c:8e:3c','00:00:03','23:57:04'),(28,'2016-08-27','财务','28:f3:66:24:8f:13','09:48:03','17:48:04'),(29,'2016-08-27','web服务器','a4:ba:db:4d:19:43','00:00:03','23:57:04'),(30,'2016-08-27','刘德山phone','cc:29:f5:8a:88:68','13:24:04','20:06:04'),(31,'2016-08-27','刘德山','80:e6:50:00:ec:26','09:21:04','20:03:04'),(32,'2016-08-27','颜恒','f4:5c:89:c4:5a:6f','10:18:04','10:18:04'),(33,'2016-08-28','Dell旧服务器','d4:ae:52:75:c8:e6','00:00:04','23:57:04'),(34,'2016-08-28','Dell新服务器','24:6e:96:0c:8e:3c','00:00:04','23:57:04'),(35,'2016-08-28','web服务器','a4:ba:db:4d:19:43','00:00:04','23:57:04'),(36,'2016-08-28','刘德山phone','cc:29:f5:8a:88:68','09:03:04','12:24:04'),(37,'2016-08-29','Dell旧服务器','d4:ae:52:75:c8:e6','00:00:04','23:57:03'),(38,'2016-08-29','Dell新服务器','24:6e:96:0c:8e:3c','00:00:04','23:57:03'),(39,'2016-08-29','财务','28:f3:66:24:8f:13','09:36:04','18:06:04'),(40,'2016-08-29','web服务器','a4:ba:db:4d:19:43','00:00:04','23:57:03'),(41,'2016-08-29','刘德山phone','cc:29:f5:8a:88:68','09:36:04','21:03:04'),(42,'2016-08-29','刘佳','a4:d1:8c:e6:82:b2','10:06:04','18:42:04'),(43,'2016-08-29','小妖','28:f0:76:37:bf:f8','09:18:03','20:57:04'),(44,'2016-08-29','刘德山','80:e6:50:00:ec:26','09:18:03','20:30:04'),(45,'2016-08-29','熊香玲','a4:d1:8c:e5:2b:a0','10:06:04','18:27:04'),(46,'2016-08-29','赵虎','a4:d1:8c:e5:22:16','10:06:04','18:42:04'),(47,'2016-08-29','吴碧丽','a4:d1:8c:e5:35:c4','09:42:04','18:39:04'),(48,'2016-08-29','颜恒','f4:5c:89:c4:5a:6f','15:36:04','21:09:04');
/*!40000 ALTER TABLE `kq_day` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kq_person_info`
--

DROP TABLE IF EXISTS `kq_person_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kq_person_info` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(20) NOT NULL COMMENT '姓名',
  `mac` varchar(20) NOT NULL COMMENT 'MAC 地址',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=13 DEFAULT CHARSET=utf8 COMMENT='个人信息';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `kq_person_info`
--

LOCK TABLES `kq_person_info` WRITE;
/*!40000 ALTER TABLE `kq_person_info` DISABLE KEYS */;
INSERT INTO `kq_person_info` VALUES (1,'刘德山','80:e6:50:00:ec:26'),(2,'刘德山phone','cc:29:f5:8a:88:68'),(3,'web服务器','a4:ba:db:4d:19:43'),(4,'Dell新服务器','24:6e:96:0c:8e:3c'),(5,'Dell旧服务器','d4:ae:52:75:c8:e6'),(6,'熊香玲','a4:d1:8c:e5:2b:a0'),(7,'吴碧丽','a4:d1:8c:e5:35:c4'),(8,'赵虎','a4:d1:8c:e5:22:16'),(9,'小妖','28:f0:76:37:bf:f8'),(10,'刘佳','a4:d1:8c:e6:82:b2'),(11,'财务','28:f3:66:24:8f:13'),(12,'颜恒','f4:5c:89:c4:5a:6f');
/*!40000 ALTER TABLE `kq_person_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kq_users`
--

DROP TABLE IF EXISTS `kq_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kq_users` (
  `userid` mediumint(8) unsigned NOT NULL AUTO_INCREMENT COMMENT '用户id',
  `companyid` mediumint(9) NOT NULL DEFAULT '0' COMMENT '公司id',
  `pid` mediumint(9) NOT NULL DEFAULT '0' COMMENT '父id',
  `username` char(20) NOT NULL DEFAULT '' COMMENT '用户名',
  `password` char(32) NOT NULL DEFAULT '' COMMENT '密码',
  `nickname` char(20) NOT NULL DEFAULT '' COMMENT '昵称',
  `regdate` int(10) unsigned NOT NULL COMMENT '注册时间',
  `lastdate` int(11) NOT NULL DEFAULT '0' COMMENT '最后一次登录时间',
  `regip` char(15) NOT NULL DEFAULT '' COMMENT '注册ip',
  `lastip` char(15) NOT NULL DEFAULT '' COMMENT '最后一次登录ip',
  `loginnum` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT '登录次数',
  `email` char(32) NOT NULL DEFAULT '' COMMENT '邮箱',
  `mobile` char(11) NOT NULL DEFAULT '' COMMENT '手机号码',
  `islock` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否锁定',
  `vip` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否会员',
  `overduedate` int(11) NOT NULL DEFAULT '0' COMMENT '账户过期时间',
  `status` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '状态-用于软删除',
  PRIMARY KEY (`userid`),
  UNIQUE KEY `username` (`username`) USING BTREE,
  KEY `email` (`email`) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `kq_users`
--

LOCK TABLES `kq_users` WRITE;
/*!40000 ALTER TABLE `kq_users` DISABLE KEYS */;
INSERT INTO `kq_users` VALUES (5,5,0,'shan275','dc483e80a7a0bd9ef71d8cf973673924','欧耶山哥',1468038500,1473161010,'127.0.0.1','192.168.0.128',28,'cooper@haiyaotech.com','15915492613',0,0,0,0);
/*!40000 ALTER TABLE `kq_users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-06-05 17:21:19
