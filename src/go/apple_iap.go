// Copyright © 2017 ichenq@outlook.com All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

package sdkutil

// Apple iTunes In-App-Purchase
// https://developer.apple.com/library/content/releasenotes/General/ValidateAppStoreReceipt/Chapters/ValidateRemotely.html

const (
	AppleIAPSandboxVerifyUrl     = "https://sandbox.itunes.apple.com/verifyReceipt"
	ApppleIAPProductionVerifyUrl = "https://buy.itunes.apple.com/verifyReceipt"
)

// 21100-21199 Internal data access error.
const (
	IAP_STATUS_INVALID_JSON     = 21000 // The App Store could not read the JSON object you provided.
	IAP_STATUS_INVALID_DATA     = 21002 // The data in the receipt-data property was malformed or missing.
	IAP_STATUS_AUTHENTICATE     = 21003 // The receipt could not be authenticated.
	IAP_STATUS_SECRET_MISMATCH  = 21004 // The shared secret you provided does not match the shared secret on file for your account.
	IAP_STATUS_SERVER_NOT_AVAIL = 21005 // The receipt server is not currently available.
	IAP_STATUS_SUB_EXPIRED      = 21006 // This receipt is valid but the subscription has expired.
	IAP_STATUS_ENV_TEST         = 21007 // This receipt is from the test environment, but it was sent to the production environment for verification.
	IAP_STATUS_ENV_PRODUCT      = 21008 // This receipt is from the production environment, but it was sent to the test environment for verification.
	IAP_STTAUS_AUTHORIZE        = 21010 // This receipt could not be authorized. Treat this the same as if a purchase was never made.
)

// https://developer.apple.com/library/content/releasenotes/General/ValidateAppStoreReceipt/Chapters/ReceiptFields.html

type InAppReceipts struct {
	Quantity                  string `json:"quantity"`
	ProductID                 string `json:"product_id"`
	TransactionID             string `json:"transaction_id"`
	OriginalTransactionID     string `json:"original_transaction_id"`
	PurchaseDate              string `json:"purchase_date"`
	PurchaseDateMs            string `json:"purchase_date_ms"`
	PurchaseDatePst           string `json:"purchase_date_pst"`
	OriginalPurchaseDate      string `json:"original_purchase_date"`
	OriginalPurchaseMs        string `json:"original_purchase_ms"`
	OriginalPurchasePst       string `json:"original_purchase_pst"`
	ExpireDate                string `json:"expires_date"`
	ExpireIntent              string `json:"expiration_intent"`
	IsInBillingRetryPeriod    string `json:"is_in_billing_retry_period"`
	IsTrialPeriod             string `json:"is_trial_period"`
	IsInIntroOfferPeriod      string `json:"is_in_intro_offer_period"`
	CancellationDate          string `json:"cancellation_date"`
	CancellationReason        string `json:"cancellation_reason"`
	AppItemID                 string `json:"app_item_id"`
	VersionExternalIdentifier string `json:"version_external_identifier"`
	WebOrderLineItemID        string `json:"web_order_line_item_id"`
	AutoRenewStatus           string `json:"auto_renew_status"`
	AutoRenewProductID        string `json:"auto_renew_product_id"`
	PriceConsentStatus        string `json:"price_consent_status"`
}

type IAPReceipt struct {
	ReceiptType          string           `json:"receipt_type"`
	AdamID               int              `json:"adam_id"`
	AppItemID            int              `json:"app_item_id"`
	DownloadID           int              `json:"download_id"`
	BundleID             string           `json:"bundle_id"`
	AppVersion           string           `json:"application_version"`
	OriginalAppVersion   string           `json:"original_application_version"`
	CreationDate         string           `json:"creation_date"`
	ExpirationDate       string           `json:"expiration_date"`
	OriginalPurchaseDate string           `json:"original_purchase_date"`
	InApps               []*InAppReceipts `json:"in_app"`
}

type IAPResponse struct {
	Status      int        `json:"status"`
	Environment string     `json:"environment"`
	IsRetryable bool       `json:"is_retryable"`
	Receipt     IAPReceipt `json:"receipt"`
}

// Sample response:
//
//{
//　　"status":0,
//　　"environment":"Sandbox",
//　　"receipt":{
//　　　　"receipt_type":"ProductionSandbox",
//　　　　"adam_id":0,
//　　　　"app_item_id":0,
//　　　　"bundle_id":"com.beyondtech.xjzhgOnLine",
//　　　　"application_version":"1.0",
//　　　　"download_id":0,
//　　　　"version_external_identifier":0,
//　　　　"receipt_creation_date":"2018-04-12 03:00:38 Etc/GMT",
//　　　　"receipt_creation_date_ms":"1523502038000",
//　　　　"receipt_creation_date_pst":"2018-04-11 20:00:38 America/Los_Angeles",
//　　　　"request_date":"2018-04-12 05:29:17 Etc/GMT",
//　　　　"request_date_ms":"1523510957951",
//　　　　"request_date_pst":"2018-04-11 22:29:17 America/Los_Angeles",
//　　　　"original_purchase_date":"2013-08-01 07:00:00 Etc/GMT",
//　　　　"original_purchase_date_ms":"1375340400000",
//　　　　"original_purchase_date_pst":"2013-08-01 00:00:00 America/Los_Angeles",
//　　　　"original_application_version":"1.0",
//　　　　"in_app":[
//　　　　　　{
//　　　　　　　　"quantity":"1",
//　　　　　　　　"product_id":"60Diamond",
//　　　　　　　　"transaction_id":"1000000390298783",
//　　　　　　　　"original_transaction_id":"1000000390298783",
//　　　　　　　　"purchase_date":"2018-04-12 03:00:36 Etc/GMT",
//　　　　　　　　"purchase_date_ms":"1523502036000",
//　　　　　　　　"purchase_date_pst":"2018-04-11 20:00:36 America/Los_Angeles",
//　　　　　　　　"original_purchase_date":"2018-04-12 03:00:36 Etc/GMT",
//　　　　　　　　"original_purchase_date_ms":"1523502036000",
//　　　　　　　　"original_purchase_date_pst":"2018-04-11 20:00:36 America/Los_Angeles",
//　　　　　　　　"is_trial_period":"false"
//　　　　　　}
//　　　　]
//　　}
//}
