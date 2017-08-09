package main

import (
	"bytes"
	"crypto/md5"
	"database/sql"
	"encoding/gob"
	"encoding/hex"
	"fmt"
	"log"
	"time"

	_ "github.com/go-sql-driver/mysql"
)

var (
	host   = "localhost"
	port   = 3306
	user   = "root"
	passwd = "holyshit"
	schema = "test"
)

// CREATE TABLE `player` (
//   `id` int(11) NOT NULL,
//   `blob` blob NOT NULL,
//   PRIMARY KEY (`id`)
// ) ENGINE=InnoDB DEFAULT CHARSET=utf8;
var debugDSN = fmt.Sprintf("%s:%s@tcp(%s:%d)/%s?charset=utf8&interpolateParams=true&parseTime=true&loc=Local", user, passwd, host, port, schema)

type Player struct {
	Uid       uint64    //角色唯一ID
	Account   string    //玩家账号
	Name      string    //角色昵称
	Level     uint16    //角色等级
	Exp       uint32    //经验
	Diamond   uint32    //钻石数量
	BirthDate time.Time //角色创建日期
}

func (p *Player) EqualTo(rhs *Player) bool {
	return p.Uid == rhs.Uid && p.Account == rhs.Account &&
		p.Name == rhs.Name && p.Level == rhs.Level &&
		p.Exp == rhs.Exp && p.Diamond == rhs.Diamond &&
		p.BirthDate == rhs.BirthDate
}

func saveToDB(db *sql.DB, player *Player) error {
	var buf bytes.Buffer
	var enc = gob.NewEncoder(&buf)
	if err := enc.Encode(player); err != nil {
		return err
	}
	fmt.Printf("write data checksum: %x\n", md5.Sum(buf.Bytes()))
	var stmt = "REPLACE INTO `player`(`id`, `blob`) VALUES(?, ?)"
	if _, err := db.Exec(stmt, player.Uid, buf.Bytes()); err != nil {
		return err
	}
	return nil
}

func loadFromDB(db *sql.DB, id uint64) (*Player, error) {
	var rawdata []byte
	if err := db.QueryRow("SELECT `blob` FROM `player` WHERE `id`=?", id).Scan(&rawdata); err != nil {
		return nil, err
	}
	fmt.Printf("load data checksum: %x\n", md5.Sum(rawdata))
	var dec = gob.NewDecoder(bytes.NewReader(rawdata))
	var player Player
	if err := dec.Decode(&player); err != nil {
		return nil, err
	}
	return &player, nil
}

func loadHexFromDB(db *sql.DB, id uint64) (string, error) {
	var rawdata []byte
	if err := db.QueryRow("SELECT `blob` FROM `player` WHERE `id`=?", id).Scan(&rawdata); err != nil {
		return "", err
	}
	fmt.Printf("hex: %v\n", hex.EncodeToString(rawdata))
	return hex.EncodeToString(rawdata), nil
}

func decodeHexBinary(hexdata string) (*Player, error) {
	bindata, err := hex.DecodeString(hexdata)
	if err != nil {
		return nil, err
	}
	var dec = gob.NewDecoder(bytes.NewReader(bindata))
	var player Player
	if err := dec.Decode(&player); err != nil {
		return nil, err
	}
	return &player, nil
}

func main() {
	db, err := sql.Open("mysql", debugDSN)
	if err != nil {
		log.Fatalf("sql.Open: %v", err)
	}
	if err := db.Ping(); err != nil {
		log.Fatalf("Ping: %v", err)
	}
	defer db.Close()

	var p1 = &Player{
		Uid:       123456789,
		Account:   "test001",
		Name:      "Pony",
		Level:     34,
		Exp:       12000,
		Diamond:   88888,
		BirthDate: time.Date(2017, 1, 1, 1, 1, 0, 0, time.UTC),
	}
	fmt.Printf("origin: %v\n", *p1)

	if err := saveToDB(db, p1); err != nil {
		log.Fatalf("%v", err)
	}

	// hexdata, err := loadHexFromDB(db, p1.Uid)
	// if err != nil {
	// 	log.Fatalf("%v", err)
	// }

	hexdata := "5eff8103010106506c6179657201ff82000107010355696401060001074163636f756e74010c0001044e616d65010c0001054c6576656c010600010345787001060001074469616d6f6e64010600010942697274684461746501ff8400000010ff830501010454696d6501ff8400000034ff8201fc075bcd150107746573743030310104506f6e79012201fe2ee001fd015b38010f010000000ecffa4bcc00000000ffff00"
	p2, err := decodeHexBinary(hexdata)
	if err != nil {
		log.Fatalf("%v", err)
	}
	fmt.Printf("loaded: %v\n", *p2)
	if p1.EqualTo(p2) {
		fmt.Printf("equal\n")
	} else {
		fmt.Printf("not equal\n")
	}
}
