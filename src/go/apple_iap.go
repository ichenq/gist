package sdk

// Apple iTunes In-App-Purchase

const (
	AppleIAPSandboxVerifyUrl     = "https://sandbox.itunes.apple.com/verifyReceipt"
	ApppleIAPProductionVerifyUrl = "https://buy.itunes.apple.com/verifyReceipt "
)

type InAppOrder struct {
	Quantity                string `json:"quantity"`
	ProductID               string `json:"product_id"`
	TransactionID           string `json:"transaction_id"`
	OriginalTransactionID   string `json:"original_transaction_id"`
	PurchaseDate            string `json:"purchase_date"`
	PurchaseDateMs          string `json:"purchase_date_ms"`
	PurchaseDatePst         string `json:"purchase_date_pst"`
	OriginalPurchaseDate    string `json:"original_purchase_date"`
	OriginalPurchaseDateMs  string `json:"original_purchase_date_ms"`
	OriginalPurchaseDatePst string `json:"original_purchase_date_pst"`
	IsTrialPeriod           string `json:"is_trial_period"`
}


type IAPReceiptV2 struct {
	ReceiptType                string        `json:"receipt_type"`
	AdamID                     int           `json:"adam_id"`
	AppItemID                  int           `json:"app_item_id"`
	BundleID                   string        `json:"bundle_id"`
	ApplicationVersion         string        `application_version`
	DownloadID                 int           `json:"download_id"`
	VersionExternalIdentifier  int           `json:"version_external_identifier"`
	ReceiptCreationDate        string        `json:"receipt_creation_date"`
	ReceiptCreationDateMs      string        `json:"receipt-creation_date_ms"`
	ReceiptCreationDateOPst    string        `json:"receipt-creation_date_pst"`
	RequestDate                string        `json:"request_date"`
	RequestDateMs              string        `json:"request_date_ms"`
	RequestDatePst             string        `json:"request_date_pst"`
	OriginalPurchaseDate       string        `json:"original_purchase_date"`
	OriginalPurchaseDateMs     string        `json:"original_purchase_date_ms"`
	OriginalPurchaseDatePst    string        `json:"original_purchase_date_pst"`
	OriginalApplicationVersion string        `json:"original_application_version"`
	InApp                      []*InAppOrder `json:"in_app"`
}

type IAPReceiptV3 struct {
	OriginalPurchaseDate    string `json:"original_purchase_date"`
	OriginalPurchaseDateMs  string `json:"original_purchase_date_ms"`
	OriginalPurchaseDatePst string `json:"original_purchase_date_pst"`
	PurchaseDate            string `json:"purchase_date"`
	PurchaseDateMs          string `json:"purchase_date_ms"`
	PurchaseDatePst         string `json:"purchase_date_pst"`
	UniqueIdentifier        string `json:"unique_identifier"`
	OriginalTransactionID   string `json:"original_transaction_id"`
	Bvrs                    string `json:"bvrs"`
	TransactionID           string `json:"transaction_id"`
	Quantity                string `json:"quantity"`
	UniqueVendorIdentifer   string `json:"unique_vendor_identifier"`
	ItemID                  string `json:"item_id"`
	ProductID               string `json:"product_id"`
	Bid                     string `json:"bid"`
}
