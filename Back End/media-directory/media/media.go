package media

import (
	"github.com/gofiber/fiber"
	"github.com/jinzhu/gorm"
	"github.com/superDeano/media-directory/dao"
	"net/http"
)

type Media struct {
	gorm.Model
	Name      string `gorm:"primaryKey" json:"name"`
	AccountId string `gorm:"notNull" json:"accountId"`
}


func GetMedias(f *fiber.Ctx)  {
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
	f.JSON(medias)
}

func InsertMedia(f *fiber.Ctx)  {
	name := f.Params("name")
	if ExistsWithName(name) {
		 f.JSON(false)
	}
	db := dao.Db
	media := new(Media)
	media.Name = name
	media.AccountId = f.Params("accId")
	db.Save(media)
	 f.JSON(true)
}

func DeleteMedia(f *fiber.Ctx)  {
	name := f.Params("name")
	if ExistsWithName(name) {
		db := dao.Db
		db.Exec("delete from doorhub.media m where m.name = ?", name)
		f.JSON(true)
	}
	 f.JSON(false)
}

func DeleteMedias(f *fiber.Ctx)  {
	accId := f.Params("accId")
	db := dao.Db
	db.Exec("delete from doorhub.media m where m.account_id = ?", accId)
	 f.SendString("Deleted All Records")
}

func GetAppHealth(f *fiber.Ctx)  {
	err := dao.Db.DB().Ping()
	if err != nil {
		 f.SendStatus(http.StatusInternalServerError)
	}
	 f.SendStatus(http.StatusOK)
}

func NameExists(f *fiber.Ctx)  {
	_ = f.JSON(ExistsWithName(f.Params("name")))
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
