package media

import (
	"../dao"
	"github.com/gofiber/fiber"
	"gorm.io/gorm"
)

type Media struct {
	gorm.Model
	Name string `json:"name"`
	AccountId string `json:"accountId"`
}

func GetMedias(f *fiber.Ctx) error {
	db := dao.Db
	accountId := f.Params("id")
	var medias []Media
	db.Find(&medias, db.Where("accountId = ?", accountId))
	return f.JSON(medias)
}

func InsertMedia(f *fiber.Ctx) error {
	return f.SendString("Insert")
}

func DeleteMedia(f *fiber.Ctx) error {
	return nil
}

func DeleteMedias(f *fiber.Ctx) error {
	return nil
}

func GetAppHealth(f *fiber.Ctx) error {
	return f.SendString("App is up and running!")
}
