#include <linux/kernel.h>
#include <linux/module.h>
#include <net/lib80211.h>

static int init(void)
{
	struct lib80211_crypto_ops *ops;
	struct lib80211_crypt_data crypt;
	struct sk_buff *skb;
	int i, err;
	static const struct {
		char *name;
		char *key;
		u8 keylen;
	} algos[] = {
		{
			.name = "WEP",
			.key = "asdfx",
			.keylen = 5,
		},
		{
			.name = "TKIP",
			.key = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
			.keylen = 32,
		}
	};
	static const struct ieee80211_hdr_3addr hdr = {
		.frame_control = cpu_to_le16(IEEE80211_FTYPE_DATA |
					     IEEE80211_STYPE_DATA),
	};

	for (i = 0; i < ARRAY_SIZE(algos); i++) {
		ops = lib80211_get_crypto_ops(algos[i].name);
		if (!ops) {
			printk(KERN_INFO "failed to look up %s\n", algos[i].name);
			continue;
		}

		crypt.ops = ops;
		crypt.priv = crypt.ops->init(0);
		if (!crypt.priv) {
			printk(KERN_INFO "failed to init %s\n", algos[i].name);
			continue;
		}

		err = crypt.ops->set_key(algos[i].key, algos[i].keylen,
					 "\x00\x00\x00\x00\x00\x00\x00\x00",
					 crypt.priv);
		if (err) {
			printk(KERN_INFO "failed to set key for %s (err=%d)\n", algos[i].name, err);
			goto deinit;
		}

		skb = dev_alloc_skb(2000);
		if (WARN_ON(!skb))
			goto deinit;

		skb_put_data(skb, &hdr, sizeof(hdr));
		skb_put_data(skb, "0123456789abcdef", 16);

		err = crypt.ops->encrypt_mpdu(skb, sizeof(hdr), crypt.priv);
		if (err) {
			printk(KERN_INFO "failed to encrypt for %s (err=%d)\n", algos[i].name, err);
			goto free;
		}

		print_hex_dump(KERN_DEBUG, algos[i].name, DUMP_PREFIX_OFFSET,
			       16, 1, skb->data, skb->len, false);

free:
		kfree_skb(skb);
deinit:
		crypt.ops->deinit(crypt.priv);
	}

	return 0;
}
module_init(init);

static void leave(void)
{
}
module_exit(leave);