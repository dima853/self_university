#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <net/if.h>

#include <linux/nl80211.h>  // Используем linux/nl80211.h
#include <netlink/genl/genl.h>
#include <netlink/genl/family.h>
#include <netlink/genl/ctrl.h>
#include <netlink/msg.h>
#include <netlink/attr.h>

// Глобальные переменные
static struct nl_sock *nl_sock;
static int nl80211_id;
static int ifindex;
static char *ifname = "wlan0"; // Замените на имя вашего Wi-Fi интерфейса

// Функция обратного вызова для обработки сообщений Netlink
static int scan_result_handler(struct nl_msg *msg, void *arg) {
    struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
    struct nlattr *attrs[NL80211_ATTR_MAX + 1];
    struct nlattr *bss[NL80211_BSS_MAX + 1];
    static struct nla_policy bss_policy[NL80211_BSS_MAX + 1] = {
        [NL80211_BSS_BSSID] = { .type = NLA_UNSPEC },
        //[NL80211_BSS_SSID] = { .type = NLA_UNSPEC }, // Закомментировано из-за ошибки
        //[NL80211_BSS_SIGNAL_DBM] = { .type = NLA_U8 }, // Закомментировано из-за ошибки
    };

    nla_parse(attrs, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL);

    if (!attrs[NL80211_ATTR_BSS]) {
        return NL_SKIP;
    }

    if (nla_parse_nested(bss, NL80211_BSS_MAX, attrs[NL80211_ATTR_BSS], bss_policy)) {
        fprintf(stderr, "Ошибка при разборе вложенных атрибутов BSS\n");
        return NL_SKIP;
    }

    // Вывод информации о сети
    //if (bss[NL80211_BSS_SSID]) { // Закомментировано из-за ошибки
    //    printf("SSID: %s\n", (char *)nla_data(bss[NL80211_BSS_SSID]));
    //}
    if (attrs[NL80211_ATTR_SSID]) {  // Попытка получить SSID из атрибута верхнего уровня
        printf("SSID: %s\n", (char *)nla_data(attrs[NL80211_ATTR_SSID]));
    }

    if (bss[NL80211_BSS_BSSID]) {
        uint8_t *bssid = (uint8_t *)nla_data(bss[NL80211_BSS_BSSID]);
        printf("BSSID: %02x:%02x:%02x:%02x:%02x:%02x\n",
               bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
    }

    //if (bss[NL80211_BSS_SIGNAL_DBM]) { // Закомментировано из-за ошибки
    //    printf("Signal (dBm): %d\n", (int8_t)nla_get_u8(bss[NL80211_BSS_SIGNAL_DBM]));
    //}

    printf("\n");

    return NL_SKIP;
}


// Функция обратного вызова для завершения сканирования
static int finish_handler(struct nl_msg *msg, void *arg) {
    int *ret = arg;
    *ret = 0;
    return NL_SKIP;
}

// Функция обратного вызова для обработки ошибок
static int error_handler(struct sockaddr_nl *nla, struct nlmsgerr *err, void *arg) {
    int *ret = arg;
    *ret = err->error;
    return NL_STOP;
}

// Функция обратного вызова для обработки сообщений о завершении
static int ack_handler(struct nl_msg *msg, void *arg) {
    int *ret = arg;
    *ret = 0;
    return NL_STOP;
}


int main() {
    int err = 0;
    struct nl_msg *msg;
    struct nl_cb *cb;

    // 1. Создание сокета Netlink
    nl_sock = nl_socket_alloc();
    if (!nl_sock) {
        fprintf(stderr, "Ошибка при выделении сокета Netlink\n");
        return 1;
    }

    // 2. Подключение к сокету Generic Netlink
    if (genl_connect(nl_sock)) {
        fprintf(stderr, "Ошибка при подключении к Generic Netlink\n");
        nl_socket_free(nl_sock);
        return 1;
    }

    // 3. Получение идентификатора семейства nl80211
    nl80211_id = genl_ctrl_resolve(nl_sock, "nl80211");
    if (nl80211_id < 0) {
        fprintf(stderr, "Ошибка при разрешении nl80211 ID\n");
        nl_socket_free(nl_sock);
        return 1;
    }

    // 4. Получение индекса интерфейса
    ifindex = if_nametoindex(ifname);
    if (!ifindex) {
        fprintf(stderr, "Ошибка при получении индекса интерфейса: %s\n", strerror(errno));
        nl_socket_free(nl_sock);
        return 1;
    }
    printf("Индекс интерфейса: %d\n", ifindex);

    // 5. Создание сообщения Netlink для запуска сканирования
    msg = nlmsg_alloc();
    if (!msg) {
        fprintf(stderr, "Ошибка при выделении сообщения Netlink\n");
        nl_socket_free(nl_sock);
        return 1;
    }

    genlmsg_put(msg, 0, 0, nl80211_id, 0, NLM_F_REQUEST, NL80211_CMD_TRIGGER_SCAN, 0);
    NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, ifindex);

    // 6. Настройка обратных вызовов
    cb = nl_cb_alloc(NL_CB_DEFAULT);
    if (!cb) {
        fprintf(stderr, "Ошибка при выделении структуры обратного вызова Netlink\n");
        nlmsg_free(msg);
        nl_socket_free(nl_sock);
        return 1;
    }

    nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, scan_result_handler, NULL);
    nl_cb_set(cb, NL_CB_FINISH, NL_CB_CUSTOM, finish_handler, &err);
    //nl_cb_set(cb, NL_CB_ERR, NL_CB_CUSTOM, error_handler, &err); // Закомментировано из-за ошибки
    nl_cb_set(cb, NL_CB_ACK, NL_CB_CUSTOM, ack_handler, &err);


    // 7. Отправка сообщения Netlink
    err = nl_send_auto(nl_sock, msg);
    if (err < 0) {
        fprintf(stderr, "Ошибка при отправке сообщения Netlink: %s\n", nl_geterror(err));
        nlmsg_free(msg);
        nl_cb_put(cb);
        nl_socket_free(nl_sock);
        return 1;
    }

    nlmsg_free(msg);

    // 8. Получение результатов сканирования
    err = 1;
    while (err > 0) {
        nl_recvmsgs(nl_sock, cb);
    }

    if (err < 0) {
        fprintf(stderr, "Ошибка при получении сообщений Netlink: %s\n", nl_geterror(err));
    }

    // 9. Запрос результатов сканирования
    msg = nlmsg_alloc();
    if (!msg) {
        fprintf(stderr, "Ошибка при выделении сообщения Netlink\n");
        nl_socket_free(nl_sock);
        nl_cb_put(cb);
        return 1;
    }

    genlmsg_put(msg, 0, 0, nl80211_id, 0, NLM_F_REQUEST | NLM_F_DUMP, NL80211_CMD_GET_SCAN, 0);
    NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, ifindex);

    nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, scan_result_handler, NULL);

     err = nl_send_auto(nl_sock, msg);
    if (err < 0) {
        fprintf(stderr, "Ошибка при отправке сообщения Netlink: %s\n", nl_geterror(err));
        nlmsg_free(msg);
        nl_cb_put(cb);
        nl_socket_free(nl_sock);
        return 1;
    }
    nlmsg_free(msg);

    err = nl_recvmsgs(nl_sock, cb);
     if (err < 0) {
        fprintf(stderr, "Ошибка при получении сообщений Netlink: %s\n", nl_geterror(err));
    }


    // 10. Освобождение ресурсов
    nl_cb_put(cb);
    nl_close(nl_sock);
    nl_socket_free(nl_sock);

    return 0;

nla_put_failure:
    fprintf(stderr, "Ошибка при создании атрибута Netlink\n");
    nlmsg_free(msg);
    nl_cb_put(cb);
    nl_socket_free(nl_sock);
    return 1;
}