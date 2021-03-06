package media

import (
	"../dao"
	"github.com/gofiber/fiber"
	"github.com/jinzhu/gorm"
	"net/http"
)

type Media struct {
	gorm.Model
	Name      string `gorm:"primaryKey" json:"name"`
	AccountId string `gorm:"notNull" json:"accountId"`
}


func GetMedias(f *fiber.Ctx) error {
	db := dao.Db
	accountId := f.Params("accId")
	var medias []string
	results, err := db.Table("media").Where("account_id = ?", accountId).Select("name").Rows()
	if err != nil {
		panic(err.Error())
	}
	var name string
	for results.Next() {
		results.Scan(&name)
		medias = append(medias, name)
	}
	return f.JSON(medias)
}

func InsertMedia(f *fiber.Ctx) error {
	name := f.Params("name")
	if ExistsWithName(name) {
		return f.JSON(false)
	}
	db := dao.Db
	media := new(Media)
	media.Name = name
	media.AccountId = f.Params("accId")
	db.Save(media)
	return f.JSON(true)
}

func DeleteMedia(f *fiber.Ctx) error {
	name := f.Params("name")
	if ExistsWithName(name) {
		db := dao.Db
		db.Exec("delete from doorhub.media m where m.name = ?", name)
		return f.JSON(true)
	}
	return f.JSON(false)
}

func DeleteMedias(f *fiber.Ctx) error {
	accId := f.Params("accId")
	db := dao.Db
	db.Exec("delete from doorhub.media m where m.account_id = ?", accId)
	return f.SendString("Deleted All Records")
}

func GetAppHealth(f *fiber.Ctx) error {
	db := dao.Db
	var err = db.Error
	if err != nil {
		return f.SendStatus(http.StatusInternalServerError)
	}
	return f.SendStatus(http.StatusOK)
}

func NameExists(f *fiber.Ctx) error {
	return f.JSON(ExistsWithName(f.Params("name")))
}

func ExistsWithName(name string) bool {
	db := dao.Db
	var count int

	results, err := db.Table("media").Where("name = ?", name).Select("COUNT(name)").Rows()
	if err != nil {
		panic(err.Error())
	}
	for results.Next() {
		_ = results.Scan(&count)
	}
	return count > 0

}
