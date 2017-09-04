-- phpMyAdmin SQL Dump
-- version 3.5.1
-- http://www.phpmyadmin.net
--
-- 主机: localhost
-- 生成日期: 2014 年 05 月 12 日 02:09
-- 服务器版本: 5.5.24-log
-- PHP 版本: 5.3.13

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- 数据库: `addb`
--

-- --------------------------------------------------------

-- --------------------------------------------------------

--
-- 表的结构 `tb_install`
--

DROP TABLE IF EXISTS `tb_install`;
CREATE TABLE IF NOT EXISTS `tb_install` (
  `ip` varchar(32) NOT NULL,
  `install_time` datetime NOT NULL,
  `install_name` varchar(32) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;


GRANT ALL PRIVILEGES ON *.* TO 'aduser'@'%' IDENTIFIED BY PASSWORD '*3C4CAC5F9299DA5727380393F02B0E30E12C376F' WITH GRANT OPTION;
GRANT ALL PRIVILEGES ON `addb`.* TO 'aduser'@'%' WITH GRANT OPTION;

GRANT ALL PRIVILEGES ON *.* TO 'aduser'@'localhost' IDENTIFIED BY PASSWORD '*3C4CAC5F9299DA5727380393F02B0E30E12C376F' WITH GRANT OPTION;
GRANT ALL PRIVILEGES ON `addb`.* TO 'aduser'@'localhost' WITH GRANT OPTION;
