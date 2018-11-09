/* https://github.com/getsentry/sentry-javascript/blob/60fee6bf/packages/types/src/index.ts#L79 */

[@bs.deriving abstract]
type config = {
  dsn: string,
};
[@bs.module "@sentry/node"] external _init : config => unit = "init";
let init = (~dsn) => {
  _init(config(~dsn));
};

[@bs.deriving abstract]
type breadcrumb = {
  [@bs.optional] type_: string,
  [@bs.optional] level: string,
  [@bs.optional] event_id: string,
  [@bs.optional] category: string,
  [@bs.optional] message: string,
  [@bs.optional] data: Js.Json.t,
  [@bs.optional] timestamp: int,
};
[@bs.module "@sentry/node"] external addBreadcrumb : breadcrumb => unit = "addBreadcrumb";

[@bs.module "@sentry/node"] external captureMessage : string => unit = "captureMessage";

[@bs.deriving abstract]
type event = {
  [@bs.optional] event_id: string,
  [@bs.optional] message: string,
  [@bs.optional] timestamp: int,
  [@bs.optional] level: string,
  [@bs.optional] platform: string,
  [@bs.optional] logger: string,
  [@bs.optional] server_name: string,
  [@bs.optional] release: string,
  [@bs.optional] dist: string,
  [@bs.optional] environment: string,
  [@bs.optional] sdk: Js.Json.t, /* TODO: type out SdkInfo Object */
  [@bs.optional] request: Js.Json.t, /* TODO: type the request object */
  [@bs.optional] transaction: string,
  [@bs.optional] modules: Js.Dict.t(string),
  [@bs.optional] fingerprint: array(string),
  [@bs.optional] stacktrace: Js.Json.t, /* TODO: type stacktrace object */
  [@bs.optional] breadcrumbs: array(breadcrumb),
  [@bs.optional] context: Js.Json.t, /* TODO: type context */
  [@bs.optional] tags: Js.Json.t, /* TODO: type tags */
  [@bs.optional] extra: Js.Json.t, /* TODO: type extra */
  [@bs.optional] user: Js.Json.t, /* TODO: type user */
};
[@bs.module "@sentry/node"] external captureEvent : event => unit = "captureEvent";

